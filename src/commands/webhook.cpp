#include "commands/webhook.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using clock_c = chrono::steady_clock;

const string WEBHOOK_FILE = "input/webhooks.txt";

static mutex printMutex;

static mutex rateMutex;
static clock_c::time_point nextAllowed = clock_c::now();

struct Job {
  string url;
  string payload;
};

static bool targetAllowed(const string &url) {
  const char *env = getenv("HYDRION_ALLOW_EXTERNAL");
  bool allowExternal = env && string(env) == "1";

  if (url.find("localhost") != string::npos ||
      url.find("127.0.0.1") != string::npos)
    return true;
  return allowExternal;
}

static bool sendHttpPostWithHandle(CURL *curl, struct curl_slist *headers,
                                   const string &url, const string &jsonPayload,
                                   long timeoutMs = 5000L) {
  if (!curl)
    return false;

  curl_easy_reset(curl);
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE,
                   (curl_off_t)jsonPayload.size());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  CURLcode res = curl_easy_perform(curl);
  long response_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

  {
    lock_guard<mutex> lock(printMutex);
    if (res != CURLE_OK) {
      cerr << "\033[31m[ERROR]\033[0m curl failed: " << curl_easy_strerror(res)
           << " -> " << url << "\n";
    } else {
      cout << "\033[32m[INFO]\033[0m HTTP " << response_code << " -> " << url
           << "\n";
    }
  }

  return (res == CURLE_OK) && (response_code >= 200 && response_code < 300);
}

static void workerFunc(queue<Job> &jobs, mutex &jobsMutex,
                       condition_variable &jobsCv, atomic<bool> &producingDone,
                       double intervalMs, long timeoutMs) {
  CURL *curl = curl_easy_init();
  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  if (!curl) {
    lock_guard<mutex> lock(printMutex);
    cerr << "\033[31m[ERROR]\033[0m Failed to init libcurl handle in worker\n";
    return;
  }

  while (true) {
    Job job;
    {
      unique_lock<mutex> lock(jobsMutex);
      jobsCv.wait(lock, [&] { return !jobs.empty() || producingDone.load(); });
      if (jobs.empty() && producingDone.load())
        break;
      if (jobs.empty())
        continue;
      job = std::move(jobs.front());
      jobs.pop();
    }

    if (intervalMs > 0.0) {
      unique_lock<mutex> rlock(rateMutex);
      auto now = clock_c::now();
      if (now < nextAllowed) {
        auto waitDur = nextAllowed - now;
        rlock.unlock();
        this_thread::sleep_for(waitDur);
        rlock.lock();
      }
      nextAllowed = max(nextAllowed, clock_c::now()) +
                    chrono::milliseconds((long)intervalMs);
    }

    sendHttpPostWithHandle(curl, headers, job.url, job.payload, timeoutMs);
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
}

void spamWebhook() {
  ifstream file(WEBHOOK_FILE);
  if (!file) {
    cout << "\033[31m[✖] No saved webhooks found!\033[0m\n";
    return;
  }

  vector<string> webhooks;
  string urlLine;
  while (getline(file, urlLine)) {
    if (!urlLine.empty())
      webhooks.push_back(urlLine);
  }
  file.close();

  if (webhooks.empty()) {
    cout << "\033[31m[✖] No valid webhooks in file!\033[0m\n";
    return;
  }

  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  cout << "\n\033[36m[>] Enter message to send: \033[0m";
  string message;
  getline(cin, message);
  if (message.empty()) {
    cout << "\033[31m[✖] Message cannot be empty!\033[0m\n";
    return;
  }

  cout << "\n\033[36m[>] Enter number of messages to send per webhook: \033[0m";
  int count;
  if (!(cin >> count)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\033[31m[✖] Invalid number!\033[0m\n";
    return;
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  if (count <= 0) {
    cout << "\033[31m[✖] Invalid message count!\033[0m\n";
    return;
  }

  cout << "\n\033[36m[>] Enter global requests-per-second limit (0 = "
          "unlimited/network-limited): \033[0m";
  double globalRps = 0.0;
  if (!(cin >> globalRps)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\033[31m[✖] Invalid RPS value!\033[0m\n";
    return;
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  cout << "\n\033[36m[>] Enter number of worker threads to use (0=auto): "
          "\033[0m";
  int threads = 0;
  if (!(cin >> threads)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\033[31m[✖] Invalid thread count!\033[0m\n";
    return;
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  if (threads <= 0) {
    threads = (int)thread::hardware_concurrency();
    if (threads <= 0)
      threads = 4;
  }

  queue<Job> jobs;
  mutex jobsMutex;
  condition_variable jobsCv;
  atomic<bool> producingDone{false};

  for (const auto &w : webhooks) {
    for (int i = 0; i < count; ++i) {
      string payload = "{\"content\":\"" + message + "\"}";
      jobs.push(Job{w, payload});
    }
  }

  double intervalMs = 0.0;
  if (globalRps > 0.0)
    intervalMs = 1000.0 / globalRps;

  if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
    cout << "\033[31m[✖] Failed to init libcurl globally\033[0m\n";
    return;
  }

  vector<thread> pool;
  for (int i = 0; i < threads; ++i) {
    pool.emplace_back(workerFunc, std::ref(jobs), std::ref(jobsMutex),
                      std::ref(jobsCv), std::ref(producingDone), intervalMs,
                      5000L /* per-request timeout ms */);
  }

  {
    lock_guard<mutex> lock(jobsMutex);
  }
  jobsCv.notify_all();

  while (true) {
    {
      lock_guard<mutex> lock(jobsMutex);
      if (jobs.empty())
        break;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  producingDone.store(true);
  jobsCv.notify_all();
  for (auto &t : pool) {
    if (t.joinable())
      t.join();
  }

  curl_global_cleanup();

  cout << "\n\033[32m[✔] Sending completed!\033[0m\n";
}

void storeWebhook() {
  cout << "\n\033[36m[>] Enter Discord Webhook URL: \033[0m";
  string webhook;
  getline(cin, webhook);

  if (webhook.empty() ||
      webhook.find("discord.com/api/webhooks/") == string::npos) {
    cout << "\033[31m[✖] Invalid Webhook URL!\033[0m\n";
    return;
  }

  ofstream file(WEBHOOK_FILE, ios::app);
  if (!file) {
    cout << "\033[31m[✖] Error saving webhook!\033[0m\n";
    return;
  }

  file << webhook << endl;
  file.close();

  cout << "\033[32m[✔] Webhook saved successfully!\033[0m\n";
}

void clearWebhooks() {
  ofstream file(WEBHOOK_FILE, ios::trunc);
  if (!file) {
    cout << "\033[31m[✖] Error clearing webhooks!\033[0m\n";
    return;
  }
  file.close();
  cout << "\033[32m[✔] All saved webhooks cleared!\033[0m\n";
}

void listWebhooks() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  ifstream file(WEBHOOK_FILE);
  if (!file) {
    cout << "\033[31m[✖] No saved webhooks found!\033[0m\n";
    return;
  }

  string line;
  for (long long ln = 1; getline(file, line); ++ln) {
    if (!line.empty()) {
      cout << "||" << ln << "||: " << line << "\n";
    }
  }
}

void deleteWebhook() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  cout << "\n\033[36m[>] Enter Webhook URL to delete: \033[0m";
  string webhook;
  getline(cin, webhook);

  const string prefix = "https://discord.com/api/webhooks/";
  if (webhook.find(prefix) != 0) {
    cout << "\033[31m[✖] Invalid Webhook URL!\033[0m\n";
    return;
  }

  string rest = webhook.substr(prefix.size());
  size_t slashPos = rest.find('/');
  if (slashPos == string::npos) {
    cout << "\033[31m[✖] Unable to parse webhook ID/token!\033[0m\n";
    return;
  }

  string id = rest.substr(0, slashPos);
  string token = rest.substr(slashPos + 1);

  cout << "\033[32m[✔] Parsed Webhook ID:\033[0m " << id << "\n";
  cout << "\033[32m[✔] Parsed Token:\033[0m " << token << "\n\n";

  cout << "\033[33m[!] Sending DELETE request to Discord...\033[0m\n";

  CURL *curl = curl_easy_init();
  if (!curl) {
    cout << "\033[31m[✖] Failed to initialize CURL!\033[0m\n";
    return;
  }

  curl_easy_setopt(curl, CURLOPT_URL, webhook.c_str());
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 5000L);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

  CURLcode res = curl_easy_perform(curl);
  long response = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK) {
    cout << "\033[31m[✖] CURL Error:\033[0m " << curl_easy_strerror(res)
         << "\n";
    return;
  }

  if (response < 200 || response >= 300) {
    cout << "\033[31m[✖] Discord returned HTTP " << response
         << "! Webhook may already be deleted.\033[0m\n";
  } else {
    cout << "\033[32m[✔] Discord Webhook deleted successfully!\033[0m\n";
  }

  ifstream infile(WEBHOOK_FILE);
  if (!infile) {
    cout << "\033[31m[✖] Could not open webhook file!\033[0m\n";
    return;
  }

  vector<string> hooks;
  string line;
  while (getline(infile, line)) {
    if (!line.empty())
      hooks.push_back(line);
  }
  infile.close();

  bool found = false;
  vector<string> updated;

  for (auto &h : hooks) {
    if (h != webhook)
      updated.push_back(h);
    else
      found = true;
  }

  if (!found) {
    cout << "\033[33m[!] Webhook was not found locally, but it was deleted "
            "remotely.\033[0m\n";
  }

  ofstream outfile(WEBHOOK_FILE, ios::trunc);
  for (auto &h : updated)
    outfile << h << "\n";
  outfile.close();

  cout << "\033[32m[✔] Webhook removed from local storage!\033[0m\n";
}

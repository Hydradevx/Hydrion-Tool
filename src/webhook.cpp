#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "../include/webhook.h"

using namespace std;

const string WEBHOOK_FILE = "webhooks.txt";

void storeWebhook() {
    cout << "\n\033[36m[>] Enter Discord Webhook URL: \033[0m";
    string webhook;
    getline(cin, webhook);

    if (webhook.empty() || webhook.find("discord.com/api/webhooks/") == string::npos) {
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

void spamWebhook() {
    ifstream file(WEBHOOK_FILE);
    if (!file) {
        cout << "\033[31m[✖] No saved webhooks found!\033[0m\n";
        return;
    }

    vector<string> webhooks;
    string url;
    while (getline(file, url)) {
        webhooks.push_back(url);
    }
    file.close();

    if (webhooks.empty()) {
        cout << "\033[31m[✖] No valid webhooks in file!\033[0m\n";
        return;
    }

    cout << "\n\033[36m[>] Enter message to spam: \033[0m";
    string message;
    getline(cin, message);

    if (message.empty()) {
        cout << "\033[31m[✖] Message cannot be empty!\033[0m\n";
        return;
    }

    cout << "\n\033[36m[>] Enter number of messages to send: \033[0m";
    int count;
    cin >> count;
    cin.ignore(); 

    if (count <= 0) {
        cout << "\033[31m[✖] Invalid message count!\033[0m\n";
        return;
    }

    cout << "\n\033[33m[!] Spamming " << count << " messages to " << webhooks.size() << " webhooks...\033[0m\n";

    vector<thread> threads;
    for (const auto &hook : webhooks) {
        threads.emplace_back([hook, message, count]() {
            for (int i = 0; i < count; i++) {
#ifdef _WIN32
                string command = "curl -s -X POST -H \"Content-Type: application/json\" -d \"{\\\"content\\\": \\\"" + message + "\\\"}\" " + hook + " > nul";
#else
                string command = "curl -s -X POST -H 'Content-Type: application/json' -d '{\"content\": \"" + message + "\"}' " + hook + " > /dev/null 2>&1";
#endif
                system(command.c_str());
            }
        });
    }

    for (auto& th : threads) th.join();

    cout << "\033[32m[✔] Spamming completed!\033[0m\n";
}
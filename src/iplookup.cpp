#include <iostream>
#include <fstream>
#include <regex>
#include "../include/iplookup.h"

using namespace std;

string extractValue(const string &json, const string &key) {
    regex pattern("\"" + key + "\":\"([^\"]+)\"");
    smatch match;
    if (regex_search(json, match, pattern))
        return match[1];
    return "Unknown";
}

void ipLookup() {
    cout << "\n\033[36m[>] Enter IP address: \033[0m";
    string ip;
    getline(cin, ip);

    if (ip.empty()) {
        cout << "\033[31m[✖] Invalid IP!\033[0m\n";
        return;
    }

    string command;
#ifdef _WIN32
    command = "powershell -Command \"Invoke-WebRequest -Uri 'http://ip-api.com/json/" + ip + "' -OutFile ip_info.txt\"";
#else
    command = "curl -s http://ip-api.com/json/" + ip + " -o ip_info.txt";
#endif

    system(command.c_str());

    ifstream file("ip_info.txt");
    if (!file) {
        cout << "\033[31m[✖] Failed to fetch IP details!\033[0m\n";
        return;
    }

    string json((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

#ifdef _WIN32
    system("del ip_info.txt");
#else
    system("rm ip_info.txt");
#endif

    string status = extractValue(json, "status");
    if (status != "success") {
        cout << "\033[31m[✖] IP not found or invalid!\033[0m\n";
        return;
    }

    string country = extractValue(json, "country");
    string region = extractValue(json, "regionName");
    string city = extractValue(json, "city");
    string isp = extractValue(json, "isp");
    string org = extractValue(json, "org");

    cout << "\n\033[36m╔══════════════════════════════════════╗\033[0m";
    cout << "\n\033[36m║         IP Lookup Result             ║\033[0m";
    cout << "\n\033[36m╠══════════════════════════════════════╣\033[0m";
    cout << "\n\033[36m║ \033[34mIP Address   \033[36m│ \033[0m" << ip;
    cout << "\n\033[36m║ \033[34mCountry      \033[36m│ \033[0m" << country;
    cout << "\n\033[36m║ \033[34mRegion       \033[36m│ \033[0m" << region;
    cout << "\n\033[36m║ \033[34mCity         \033[36m│ \033[0m" << city;
    cout << "\n\033[36m║ \033[34mISP          \033[36m│ \033[0m" << isp;
    cout << "\n\033[36m║ \033[34mOrganization \033[36m│ \033[0m" << org;
    cout << "\n\033[36m╚══════════════════════════════════════╝\033[0m\n";
}
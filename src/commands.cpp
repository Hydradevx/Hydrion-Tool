#include <iostream>
#include <vector>
#include "../include/commands.h"
#include "../include/iplookup.h"
#include "../include/webhook.h"
#include "../include/art.h"
#include "../include/token.h"
#include "../include/stats.h"

using namespace std;

void displayPage(const vector<string>& commands, int page, int pageSize) {
    int start = page * pageSize;
    int end = min(start + pageSize, (int)commands.size());

    printHydrion();
    cout << "\n\033[34m[ Page " << page + 1 << " / " << (commands.size() + pageSize - 1) / pageSize << " ]\033[0m\n";
    for (int i = start; i < end; i++) {
        cout << "\033[32m[" << (i - start + 1) << "] " << commands[i] << "\033[0m\n";
    }
}

void executeCommand(const vector<string>& commands, int index) {
    system("cls || clear");
    if (index < 0 || index >= commands.size()) {
        cout << "\033[31m[✖] Invalid command number!\033[0m\n";
        return;
    }

    cout << "\033[32m[✔] Executing: " << commands[index] << "\033[0m\n";

    if (commands[index].find("IP Lookup") != string::npos) {
        ipLookup();
    } else if (commands[index].find("Store Webhook") != string::npos) {
        storeWebhook();
    } else if (commands[index].find("Spam Webhooks") != string::npos) {
        spamWebhook();
    } else if (commands[index].find("Add Token") != string::npos) {
        addToken();
    } else if (commands[index].find("List Tokens") != string::npos) {
        listTokens();
    } else if (commands[index].find("Remove Token") != string::npos) {
        removeToken();
    } else if (commands[index].find("User Stats") != string::npos) {
        getUserStats();
    } else if (commands[index].find("Server Stats") != string::npos) {
        getServerStats();
    }
    else {
        cout << "Command executed but has no real function yet.\n";
    }
}
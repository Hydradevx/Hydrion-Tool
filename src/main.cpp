#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <vector>
#include "../include/commands.h"
#include "../include/art.h"

using namespace std;

bool runUpdater() {
    cout << "\n🌀 Checking for updates...\n";
    int result = system("git pull");
    return result == 0;
}

int main(int argc, char* argv[]) {

    runUpdater();

    system("cls || clear");
    
    vector<string> commands = {
        "IP Lookup - Get details of an IP",
        "Store Webhook - Save a Discord webhook",
        "Spam Webhooks - Send spam messages to saved webhooks",
        "Add Token - Store a new bot token",
        "List Tokens - View stored bot tokens",
        "Remove Token - Delete a stored bot token",
        "Join Server - Join a server using an invite code",
        "User Stats - Get statistics about one of your User Tokens",
        "Server Stats - Get statistics about a Server with it's Invite Code"
    };

    int page = 0, pageSize = 5;

    while (true) {
        displayPage(commands, page, pageSize);

        cout << "\n\033[36m[>] Enter command number, < (prev), > (next), X (exit): \033[0m";
        string input;
        getline(cin, input);

        if (input == "X" || input == "x") {
            cout << "\033[32m[✔] Exiting...\033[0m\n";
            break;
        } else if (input == "<" && page > 0) {
            system("cls || clear");
            page--;
        } else if (input == ">" && (page + 1) * pageSize < commands.size()) {
            system("cls || clear");
            page++;
        } else {
            try {
                int choice = stoi(input);
                int commandIndex = page * pageSize + (choice - 1);
                executeCommand(commands, commandIndex);
            } catch (...) {
                cout << "\033[31m[✖] Invalid input!\033[0m\n";
            }
        }
    }

    return 0;
}
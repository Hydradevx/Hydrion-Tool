#include <iostream>
#include <fstream>
#include <vector>
#include "../include/token.h"

using namespace std;

const string TOKEN_FILE = "tokens.txt";

void addToken() {
    cout << "\n\033[36m[>] Enter bot token: \033[0m";
    string token;
    getline(cin, token);

    if (token.empty()) {
        cout << "\033[31m[✖] Token cannot be empty!\033[0m\n";
        return;
    }

    ofstream file(TOKEN_FILE, ios::app);
    if (!file) {
        cout << "\033[31m[✖] Error saving token!\033[0m\n";
        return;
    }

    file << token << endl;
    file.close();

    cout << "\033[32m[✔] Token saved successfully!\033[0m\n";
}

void listTokens() {
    ifstream file(TOKEN_FILE);
    if (!file) {
        cout << "\033[31m[✖] No stored tokens found!\033[0m\n";
        return;
    }

    vector<string> tokens;
    string line;
    while (getline(file, line)) {
        tokens.push_back(line);
    }
    file.close();

    if (tokens.empty()) {
        cout << "\033[31m[✖] No tokens saved!\033[0m\n";
        return;
    }

    cout << "\n\033[32m[✔] Stored Bot Tokens:\033[0m\n";
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << "\033[34m[" << i + 1 << "] ••••••••••••••••••••••••••\033[0m\n";
    }
}

void removeToken() {
    ifstream file(TOKEN_FILE);
    if (!file) {
        cout << "\033[31m[✖] No tokens stored!\033[0m\n";
        return;
    }

    vector<string> tokens;
    string line;
    while (getline(file, line)) {
        tokens.push_back(line);
    }
    file.close();

    if (tokens.empty()) {
        cout << "\033[31m[✖] No tokens found!\033[0m\n";
        return;
    }

    listTokens();
    cout << "\n\033[36m[>] Enter the token number to remove: \033[0m";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > tokens.size()) {
        cout << "\033[31m[✖] Invalid choice!\033[0m\n";
        return;
    }

    tokens.erase(tokens.begin() + (choice - 1));

    ofstream outFile(TOKEN_FILE);
    for (const auto& t : tokens) {
        outFile << t << endl;
    }
    outFile.close();

    cout << "\033[32m[✔] Token removed successfully!\033[0m\n";
}
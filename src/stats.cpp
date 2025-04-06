#include "../include/stats.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

void getUserStats() {
    std::string token = chooseToken();
    if (token.empty()) {
        return;
    }

    std::string command = "curl -s -X GET \"https://discord.com/api/v10/users/@me\" "
                          "-H \"Authorization: " + token + "\" "
                          "-H \"Content-Type: application/json\"";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "\033[31m[!] Failed to execute command!\033[0m\n";
        return;
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    try {
        nlohmann::json jsonData = nlohmann::json::parse(result);
        std::cout << "\n\033[32m[+] Username: " << jsonData["username"].get<std::string>() << "\033[0m\n";
        std::cout << "\033[32m[+] ID: " << jsonData["id"].get<std::string>() << "\033[0m\n";
        std::cout << "\033[32m[+] Discriminator: " << jsonData["discriminator"].get<std::string>() << "\033[0m\n";
        std::cout << "\033[32m[+] Avatar URL: " << "https://cdn.discordapp.com/avatars/" + jsonData["id"].get<std::string>() + "/" + jsonData["avatar"].get<std::string>() << "\033[0m\n";
        std::cout << "\033[32m[+] Nitro Status: " << (jsonData["premium_type"].get<int>() == 2 ? "Boost" : "Basic") << "\033[0m\n";
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "\033[31m[!] JSON Parsing Error: " << e.what() << "\033[0m\n";
    }
}

void getServerStats() {
    std::string inviteCode;
    std::cout << "\n\033[36m[>] Enter the invite code: \033[0m";
    std::getline(std::cin, inviteCode);

    std::string command = "curl -s -X GET \"https://discord.com/api/v10/invites/" + inviteCode + "\" "
                          "-H \"Content-Type: application/json\"";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "\033[31m[!] Failed to execute command!\033[0m\n";
        return;
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    try {
        nlohmann::json jsonData = nlohmann::json::parse(result);
        std::cout << "\n\033[32m[+] Server Name: " << jsonData["guild"]["name"].get<std::string>() << "\033[0m\n";
        std::cout << "\n\033[32m[+] Server ID: " << jsonData["guild"]["id"].get<std::string>() << "\033[0m\n";
        std::cout << "\n\033[32m[+] Server Vanity: " 
                  << (jsonData["guild"]["vanity_url_code"].is_string() ? jsonData["guild"]["vanity_url_code"].get<std::string>() : "none") 
                  << "\033[0m\n";
        std::cout << "\n\033[32m[+] Server Boosts: " << jsonData["guild"]["premium_subscription_count"].get<int>() << "\033[0m\n";
    } catch (const std::exception& e) {
        std::cerr << "\033[31m[!] Error: " << e.what() << "\033[0m\n";
    }
}

std::string chooseToken() {
    std::ifstream file("tokens.txt");
    if (!file) {
        std::cerr << "\033[31m[!] No tokens found!\033[0m\n";
        return "";
    }

    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(file, token)) {
        token.erase(0, token.find_first_not_of(" \n\r\t"));
        token.erase(token.find_last_not_of(" \n\r\t") + 1);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    file.close();

    if (tokens.empty()) {
        std::cerr << "\033[31m[!] No valid tokens found!\033[0m\n";
        return "";
    }

    std::cout << "\n\033[36m[>] Choose a token (1 to " << tokens.size() << "): \033[0m";
    int choice;
    std::cin >> choice;

    if (choice < 1 || choice > tokens.size()) {
        std::cerr << "\033[31m[!] Invalid choice!\033[0m\n";
        return "";
    }

    return tokens[choice - 1];
}
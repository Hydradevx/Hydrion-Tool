#include "art.hpp"
#include "commands.hpp"
#include <chrono>
#include <iostream>
#include <limits>
#include <string>
#include <thread>

using namespace std;

void TextArt() {
  cout << "\033[2J";
  cout << "\033[" << 0 << ";" << 0 << "H";

  string art =
      R"(██░ ██▓██   ██▓▓█████▄  ██▀███   ██▓ ▒█████   ███▄    █ 
▓██░ ██▒▒██  ██▒▒██▀ ██▌▓██ ▒ ██▒▓██▒▒██▒  ██▒ ██ ▀█   █ 
▒██▀▀██░ ▒██ ██░░██   █▌▓██ ░▄█ ▒▒██▒▒██░  ██▒▓██  ▀█ ██▒
░▓█ ░██  ░ ▐██▓░░▓█▄   ▌▒██▀▀█▄  ░██░▒██   ██░▓██▒  ▐▌██▒
░▓█▒░██▓ ░ ██▒▓░░▒████▓ ░██▓ ▒██▒░██░░ ████▓▒░▒██░   ▓██░
 ▒ ░░▒░▒  ██▒▒▒  ▒▒▓  ▒ ░ ▒▓ ░▒▓░░▓  ░ ▒░▒░▒░ ░ ▒░   ▒ ▒ 
 ▒ ░▒░ ░▓██ ░▒░  ░ ▒  ▒   ░▒ ░ ▒░ ▒ ░  ░ ▒ ▒░ ░ ░░   ░ ▒░
 ░  ░░ ░▒ ▒ ░░   ░ ░  ░   ░░   ░  ▒ ░░ ░ ░ ▒     ░   ░ ░ 
 ░  ░  ░░ ░        ░       ░      ░      ░ ░           ░ 
        ░ ░      ░)";
  cout << art << endl;
}

void LoadPage() {
  while (true) {
    TextArt();
    cout << "\n\n";

    for (const auto &command : descriptions) {
      cout << command.first << " - " << command.second << "\n";
    }

    cout << "\nEnter command number (or -1 to exit): ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid input! Try again.\n";
      this_thread::sleep_for(chrono::seconds(1));
      continue;
    }

    if (choice == -1)
      break;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice < 0 || choice >= ((int)descriptions.size() + 1)) {
      cout << "Invalid choice. Try again.\n";
      this_thread::sleep_for(chrono::seconds(1));
      continue;
    }

    cout << "\033[2J";
    cout << "\033[" << 0 << ";" << 0 << "H";

    commands[descriptions[choice]]();

    cout << "\nPress Enter to return...";
    cin.get(); // Wait before refreshing
  }
}
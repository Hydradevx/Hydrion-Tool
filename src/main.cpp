#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>

using namespace std;

bool runUpdater() {
    cout << "\n🌀 Checking for updates...\n";
    int result = system("git pull");
    return result == 0;
}

int main(int argc, char* argv[]) {

    runUpdater();

    return 0;
}
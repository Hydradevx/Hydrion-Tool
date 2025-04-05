#include <cstdlib>
#include <iostream>

int main() {
    std::cout << "🔁 Starting update process..." << std::endl;

    if (std::system("git pull origin main") != 0) {
        std::cerr << "❌ Failed to pull from GitHub." << std::endl;
        return 1;
    }

    if (std::system("make") != 0) {
        std::cerr << "❌ Build failed." << std::endl;
        return 1;
    }

    std::cout << "✅ Successfully updated and rebuilt." << std::endl;
    return 0;
}
@echo off
echo 🔧 Setting up Hydrion Discord Tool (Windows CMD)

where git >nul 2>nul || (
    echo ❌ Git not found. Please install Git from https://git-scm.com/downloads
    pause
    exit /b
)

where g++ >nul 2>nul || (
    echo ❌ g++ not found. Please install a C++ compiler (e.g., MinGW or install via WSL).
    pause
    exit /b
)

where make >nul 2>nul || (
    echo ❌ make not found. Please install make.
    pause
    exit /b
)

git clone https://github.com/Hydradevx/discord-tool-cpp.git
cd discord-tool-cpp
make

echo ✅ Setup complete. Run the tool from bin\discordtool.exe
pause
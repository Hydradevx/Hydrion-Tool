Write-Host "🔧 Setting up Hydrion Discord Tool (Windows PowerShell)"

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "❌ Git is not installed. Download from https://git-scm.com/downloads"
    exit
}
if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "❌ g++ not found. Install via MinGW or WSL"
    exit
}
if (-not (Get-Command make -ErrorAction SilentlyContinue)) {
    Write-Host "❌ make not found. Install it or use WSL"
    exit
}

git clone https://github.com/Hydradevx/discord-tool-cpp.git
cd discord-tool-cpp
make

Write-Host "✅ Setup complete. Run the tool from bin\discordtool.exe"
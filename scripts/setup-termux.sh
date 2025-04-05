#!/data/data/com.termux/files/usr/bin/bash
echo "🔧 Setting up Hydrion Discord Tool (Termux/Android)"

pkg update && pkg install -y git clang make

git clone https://github.com/Hydradevx/Hydrion-Tool.git
cd discord-tool-cpp
make

echo "✅ Setup complete. Run with: ./bin/discordtool"
#!/bin/bash
echo "🔧 Setting up Hydrion Discord Tool (Linux)"

sudo apt update && sudo apt install -y git g++ make

git clone https://github.com/Hydradevx/Hydrion-Tool.git
cd discord-tool-cpp
make

echo "✅ Setup complete. Run with: ./bin/discordtool"

#!/bin/bash
echo "🔧 Setting up Hydrion Discord Tool (macOS)"

brew update
brew install git make gcc

git clone https://github.com/Hydradevx/discord-tool-cpp.git
cd discord-tool-cpp
make

echo "✅ Setup complete. Run with: ./bin/discordtool"
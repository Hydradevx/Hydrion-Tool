#!/bin/bash

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[1;36m'
NC='\033[0m' # No Color

function banner() {
  echo -e "${CYAN}Hydrion Discord Tool Dev Script${NC}"
}

function help_menu() {
  banner
  echo -e "${GREEN}Usage:${NC} ./scripts/dev.sh [command]"
  echo ""
  echo "Commands:"
  echo "  build     Compile the project using make"
  echo "  run       Execute the built binary"
  echo "  clean     Delete compiled binaries"
  echo "  debug     Compile and open in gdb"
  echo "  docker    Build and run Docker container"
  echo "  help      Show this menu"
}

function build() {
  banner
  echo -e "${CYAN}🔧 Building project...${NC}"
  mkdir -p bin
  make
  echo -e "${GREEN}✅ Build complete.${NC}"
}

function run() {
  banner
  echo -e "${CYAN}🚀 Running tool...${NC}"
  ./build/discordtool
}

function clean() {
  banner
  echo -e "${CYAN}🧹 Cleaning...${NC}"
  rm -rf bin/*
  echo -e "${GREEN}✅ Cleaned.${NC}"
}

function debug() {
  banner
  echo -e "${CYAN}🐞 Debugging with gdb...${NC}"
  g++ -g -o build/discordtool src/*.cpp
  gdb build/discordtool
}

function docker_run() {
  banner
  echo -e "${CYAN}🐳 Docker build & run...${NC}"
  docker build -t discordtool ./docker
  docker run --rm -it discordtool
}

case "$1" in
  build) build ;;
  run) run ;;
  clean) clean ;;
  debug) debug ;;
  docker) docker_run ;;
  help|"") help_menu ;;
  *) echo -e "${RED}Unknown command: $1${NC}"; help_menu; exit 1 ;;
esac
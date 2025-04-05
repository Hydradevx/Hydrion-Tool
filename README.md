# 💠 Hydrion Discord Tool (C++)

Cross-platform Discord tool built in C++ with auto-update, git integration, and terminal-ready.

---

## 🛠️ Installation

### 💻 Linux / WSL

```bash
curl -sL https://raw.githubusercontent.com/Hydradevx/discord-tool-cpp/main/scripts/setup-linux.sh | bash
```

---

### 📱 Android (Termux)

```bash
curl -sL https://raw.githubusercontent.com/Hydradevx/discord-tool-cpp/main/scripts/setup-termux.sh | bash
```

---

### 🍏 macOS

```bash
curl -sL https://raw.githubusercontent.com/Hydradevx/discord-tool-cpp/main/scripts/setup-macos.sh | bash
```

---

### 🪟 Windows (PowerShell)

```powershell
iwr -useb https://raw.githubusercontent.com/Hydradevx/discord-tool-cpp/main/scripts/setup-windows.ps1 | iex
```

> Or if you're using **CMD**:

```cmd
curl -LO https://raw.githubusercontent.com/Hydradevx/discord-tool-cpp/main/scripts/setup-windows.bat && setup-windows.bat
```

---

## 🔁 Update

Update manually:

```bash
curl -sL https://raw.githubusercontent.com/Hydradevx/discord-tool-cpp/main/scripts/update.sh | bash
```

Or run:

```bash
./bin/updater
```

---

## 🐳 Docker Build

```bash
docker build -t discordtool ./docker
docker run --rm discordtool
```

---

## 📂 Structure

```
discord-tool-cpp/
├── src/            # Source code (main & updater)
├── bin/            # Compiled binaries
├── scripts/        # Setup/update scripts
├── Makefile        # Build config
└── README.md       # This file
```

---

## 🧠 Requirements

- Git
- g++
- make

> On Android, Linux, and macOS, these are installed automatically.  
> On Windows, you’ll need to install manually or use WSL.

---

Need help making an interactive command interface or UI? Let me know 💡
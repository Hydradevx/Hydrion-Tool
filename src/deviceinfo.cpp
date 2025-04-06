#include <iostream>
#include <cstdlib>
#include <string>
#include <../include/deviceinfo.h>

using namespace std;
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
#endif

using namespace std;

void showDeviceInfo() {
    cout << "📱 Hydrion Device Info\n";
    cout << "────────────────────────────\n";

#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    GetComputerName(computerName, &size);

    cout << "🖥️  Hostname: " << computerName << "\n";
    cout << "🔧 CPU Cores: " << sysinfo.dwNumberOfProcessors << "\n";
    cout << "🧠 Page Size: " << sysinfo.dwPageSize << " bytes\n";
    cout << "🛠️  Processor Architecture: ";

    switch (sysinfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64: cout << "x64 (AMD or Intel)\n"; break;
        case PROCESSOR_ARCHITECTURE_INTEL: cout << "x86\n"; break;
        case PROCESSOR_ARCHITECTURE_ARM: cout << "ARM\n"; break;
        default: cout << "Unknown\n"; break;
    }

#elif __linux__ || __APPLE__ || __ANDROID__
    struct utsname sys;
    if (uname(&sys) == 0) {
        cout << "🖥️  Hostname: " << sys.nodename << "\n";
        cout << "📀 OS: " << sys.sysname << " " << sys.release << " (" << sys.machine << ")\n";
    }

    cout << "🔧 CPU Cores: " << sysconf(_SC_NPROCESSORS_ONLN) << "\n";
    cout << "🧠 RAM (pages): " << sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE) / (1024 * 1024) << " MB\n";

    system("cat /proc/uptime | cut -d' ' -f1 2>/dev/null || uptime");
    cout << "⏱️  Uptime (s): ";
#endif

    cout << "\n────────────────────────────\n";
}
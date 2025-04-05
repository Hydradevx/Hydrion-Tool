#include <iostream>
#include <thread>
#include <chrono>
#include "../include/art.h"

using namespace std;

void printHydrion() {
    string hydrion[6] = {
        "██╗  ██╗██╗   ██╗██████╗ ██████╗ ██╗ ██████╗ ███╗   ██╗",
        "██║  ██║╚██╗ ██╔╝██╔══██╗██╔══██╗██║██╔═══██╗████╗  ██║",
        "███████║ ╚████╔╝ ██║  ██║██████╔╝██║██║   ██║██╔██╗ ██║",
        "██╔══██║  ╚██╔╝  ██║  ██║██╔══██╗██║██║   ██║██║╚██╗██║",
        "██║  ██║   ██║   ██████╔╝██║  ██║██║╚██████╔╝██║ ╚████║",
        "╚═╝  ╚═╝   ╚═╝   ╚═════╝ ╚═╝  ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝"
    };

    int colors[] = {17, 18, 19, 20, 21, 27}; 

    for (int i = 0; i < 6; i++) {
        cout << "\033[38;5;" << colors[i] << "m" << hydrion[i] << "\033[0m" << endl;
        this_thread::sleep_for(chrono::milliseconds(50)); 
    }
}
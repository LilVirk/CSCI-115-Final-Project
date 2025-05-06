/**
* PlatformDetection.h
* Contains platform-specific definitions and functions for cross-platform compatibility
**/
#pragma once

#include <iostream>
#include <string>

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
    #include <Windows.h>
    #include <conio.h>
#elif defined(__APPLE__) || defined(__MACH__)
    #define PLATFORM_MAC
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#elif defined(__linux__) || defined(__unix__)
    #define PLATFORM_LINUX
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#else
    #error "Unsupported platform"
#endif

// Common key codes
#ifdef PLATFORM_WINDOWS
    // Windows uses VK_ constants from Windows.h
#else 
    // Define key constants for Mac and Linux
    const int VK_UP = 'w';     // W for up
    const int VK_DOWN = 's';   // S for down
    const int VK_RIGHT = 'd';  // D for right
    const int VK_LEFT = 'a';   // A for left
    const int VK_SPACE = ' ';
    const int VK_ESCAPE = 27;  // ESC key
    const int VK_SHIFT = 'r';  // Use 'r' for shift on Mac/Linux
#endif

// Cross-platform functions
namespace Platform {
    // Check if a key is pressed
    inline bool isKeyPressed(int key) {
    #ifdef PLATFORM_WINDOWS
        return GetAsyncKeyState(key) & 0x8000;
    #else
        struct termios oldt, newt;
        int ch;
        int oldf;
        
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        
        ch = getchar();
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        
        if (ch != EOF) {
            ungetc(ch, stdin);
            return (ch == key);
        }
        
        return false;
    #endif
    }

    // Clear the screen
    inline void clearScreen() {
    #ifdef PLATFORM_WINDOWS
        system("cls");
    #else
        system("clear");
    #endif
    }

    // Sleep for milliseconds
    inline void sleep(int milliseconds) {
    #ifdef PLATFORM_WINDOWS
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
    }

    // List files in a directory
    inline void listFiles(const std::string& directory, const std::string& extension) {
    #ifdef PLATFORM_WINDOWS
        system(("dir /b " + directory + "\\*." + extension).c_str());
    #else
        system(("ls -1 " + directory + "/*." + extension + " 2>/dev/null || echo 'Directory not found'").c_str());
    #endif
    }
}
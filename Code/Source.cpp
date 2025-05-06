#include <iostream>
#include <thread>
#include <chrono>
#include "Vector.h"
#include "GameGraph.h"
#include "CreateMap.h"
#include "Graphics.h"
#include "PlatformDetection.h"

void startGame(MapGraph currentMap) {
    int cursorX = sqrt(currentMap.width);
    int cursorY = sqrt(currentMap.height);
    std::pair<int, int> selectionPosition;
    currentMap.displayWorld();
    bool gameState = true;
    currentMap.moveCursor(cursorX,cursorY);
    
    while (gameState) {
        if (Platform::isKeyPressed(VK_UP)) {
            if (cursorY > 0) {
                cursorY--;
            }
            currentMap.moveCursor(cursorX, cursorY);
            Platform::sleep(100);
        }
        if (Platform::isKeyPressed(VK_DOWN)) {
            if (cursorY < currentMap.height-1) {
                cursorY++;
            }
            currentMap.moveCursor(cursorX, cursorY);
            Platform::sleep(100);
        }
        if (Platform::isKeyPressed(VK_RIGHT)) {
            if (cursorX < currentMap.width-1) {
                cursorX++;          
            }
            currentMap.moveCursor(cursorX, cursorY);
            Platform::sleep(100);
        }
        if (Platform::isKeyPressed(VK_LEFT)) {
            if (cursorX > 0) {
                cursorX--;
            }
            currentMap.moveCursor(cursorX, cursorY);
            Platform::sleep(100);
        }
        if (Platform::isKeyPressed('Q')) {  // Exit on Q
            std::cout << "Exiting program...\n";
            gameState = false;
            Platform::sleep(200);
        }
        if (Platform::isKeyPressed('P')) {  
            currentMap.moveUnit(selectionPosition.first, selectionPosition.second, cursorX,cursorY);
            Platform::sleep(200);
        }
        #ifdef PLATFORM_WINDOWS
        if (Platform::isKeyPressed(VK_SHIFT)) {
        #else
        if (Platform::isKeyPressed('R')) { // Use 'R' as alternative to Shift on macOS
        #endif
            currentMap.nextTurn();
            Platform::sleep(400);
        }
        if (Platform::isKeyPressed(VK_SPACE)) {  
            currentMap.selectUnit(cursorX, cursorY);
            selectionPosition.first = cursorX;
            selectionPosition.second = cursorY;
            currentMap.showMovement(cursorX, cursorY);
            currentMap.displayWorld();
            Platform::sleep(200);
        }
        if (Platform::isKeyPressed(VK_ESCAPE)) {
            currentMap.clearTileStates();
            Platform::sleep(200);
        }
        
        // Add a small sleep to prevent CPU hogging
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void loadMap(std::string mapName) {
    // makes sure map given exists 
    MapGraph currentMap(importMapFile(mapName));
    if (currentMap.isInitialized()) {
        std::cout << "\nMAP: " << currentMap.getMapName();
        startGame(currentMap); // loop for handling key input
    }
    else {
        std::cerr << "\nERROR: MAP CANNOT BE LOADED FROM FILE";
        std::cout << "\nPlease ensure the map file is in the same directory as the executable or in a Maps/ subdirectory.";
        std::cout << "\nAvailable maps: ";
        Platform::listFiles("Maps", "txt");
    }
}

int main() {
    Platform::clearScreen();
    std::string mapName;
    std::cout << "Enter Map Name (with or without .txt extension): ";
    std::cin >> mapName;
    loadMap(mapName);
    
    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}
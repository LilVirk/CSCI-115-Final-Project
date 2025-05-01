#include <iostream>
#include "Vector.h"
#include "GameGraph.h"
#include "CreateMap.h"
#include "Windows.h"
#include "Graphics.h"

void startGame(MapGraph currentMap) {
    int cursorX = sqrt(currentMap.width);
    int cursorY = sqrt(currentMap.height);
    std::pair<int, int> selectionPosition;
    currentMap.displayWorld();
    bool gameState = true;
    currentMap.moveCursor(cursorX,cursorY);
    while (gameState) {
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (cursorY > 0) {
                cursorY--;
            }
            currentMap.moveCursor(cursorX, cursorY);
            Sleep(100);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (cursorY < currentMap.height-1) {
                cursorY++;
            }
            currentMap.moveCursor(cursorX, cursorY);
            Sleep(100);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (cursorX < currentMap.width-1) {
                cursorX++;          
            }
            currentMap.moveCursor(cursorX, cursorY);
            Sleep(100);
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (cursorX > 0) {
                cursorX--;
            }
            currentMap.moveCursor(cursorX, cursorY);
            Sleep(100);
        }
        if (GetAsyncKeyState('Q') & 0x8000) {  // Exit on Q
            std::cout << "Exiting program...\n";
            gameState = false;
            Sleep(200);
        }
        if (GetAsyncKeyState('P') & 0x8000) {  
            currentMap.moveUnit(selectionPosition.first, selectionPosition.second, cursorX,cursorY);
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000) { 
            currentMap.nextTurn();
            Sleep(400);
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {  
            currentMap.selectUnit(cursorX, cursorY);
            selectionPosition.first = cursorX;
            selectionPosition.second = cursorY;
            currentMap.showMovement(cursorX, cursorY);
            currentMap.displayWorld();
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            currentMap.clearTileStates();
            Sleep(200);
        }
    }
}

void loadMap(std::string mapName) {
    // makes sure map given exists 
    MapGraph currentMap(importMapFile(mapName));
    if (currentMap.isInitialized()) {
        std::cout << "\nMAP: " << currentMap.getMapName();
        //currentMap.printAdjMatrix();
        //currentMap.Dijkstra(0,0);
        startGame(currentMap); // loop for handling key input
    }
    else {
        std::cerr << "\nERROR: MAP CANNOT BE LOADED FROM FILE";
    }
}

int main() {
    system("cls");
    std::string mapName;
    std::cout << "Enter Map Name: ";
    std::cin >> mapName;
    loadMap(mapName);
    //system("pause");
}
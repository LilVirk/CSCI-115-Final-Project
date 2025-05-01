#pragma once
#include <iostream>
#include "Data.h"
#include "Matrix.h"
#include "vector.h"
#include "Graphics.h"
#include "PriorityQueue.h"
//#include <utility>
/**
* 0 = NO TILE
* 1 = Wall
* 2 = Empty Tile
* 3 = Dirt
* 4 = Secret Passage
**/
class MapGraph {
private:
	bool initialized; // If map contains data and works properly, true, otherwise is empty and cannot be loaded, false
	std::string mapName = "UNNAMED MAP";
	Matrix<Tile> tileMatrix; // Used for displaying and saving map tiles for terrain
	Matrix<int> adjMatrix; // Used for movement through tiles // 
	Vector<Unit> units; // Array of units
	std::pair<int, int> cursorPosition; // keeps track of where cursor is
	std::pair<int, int> previousCursorPosition;
	int size;
	Unit selectedUnit;
	bool unitSelected = false;
	int turnsTaken = 0;
public:
	int width, height;
	MapGraph() {
		initialized = false;
		size = 0;
		width = 0;
		height = 0;
	}
	MapGraph(int width, int height) {
		this->width = width;
		this->height = height;
		initialized = true;
		tileMatrix.initMatrix(width,height);
		size = width * height;
		adjMatrix.initMatrix(size, size);
		// setup matrix to all 0 // 
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				adjMatrix.setCell(i, j, 0);
			}
		}
	}
	MapGraph(const MapGraph& other) {
		initialized = true;
		size = other.size;
		tileMatrix = other.tileMatrix;
		adjMatrix = other.adjMatrix;
		units = other.units;
		mapName = other.mapName;
		height = other.height;
		width = other.width;
		cursorPosition = other.cursorPosition;
		selectedUnit = other.selectedUnit;
		unitSelected = other.unitSelected;
	}
	~MapGraph() {
		
	}
	bool isInitialized() { return initialized; }
	void setMapName(std::string mapName) {
		this->mapName = mapName;
	}
	std::string getMapName() { return mapName; }
	int getSize() { return size; }
	void printAdjMatrix() {
		// Prints out the contents of the AdjMatrix to the console //
		std::cout << "\n   ";
		for (int i = 0; i < size; i++) {
			std::cout << i << " ";

		}
		std::cout << "\n";
		for (int i = 0; i < size; i++) {
			std::cout << i << "| ";
			for (int j = 0; j < size; j++) {
				std::cout << adjMatrix.getCell(i, j) << " ";
			}
			std::cout << "\n";
		}
	}
	void addTile(int x, int y, int tileType, int identifier) {
		Tile newTile(tileType);
		newTile.setPosition(x,y);
		newTile.tileIdentifier = identifier;
		tileMatrix.setCell(x,y, newTile);
	}
	void addUnit(int x, int y, int unitType, int team) {
		// Adds unit and modifies tile given by x and y to hold a tile // 
		Unit newUnit(unitType,team);
		newUnit.setPosition(x,y);
		units.push_back(newUnit);
		tileMatrix.getCell(x, y).setUnit(units.get_size() - 1);
		//std::cout << "\n(" << x << "," << y << ")";
	}
	void removeUnit(int x, int y) {
		// removes a unit from given location // 
		if (tileMatrix.getCell(x, y).containsUnit()) {
			int unitIndex = tileMatrix.getCell(x, y).getUnitIndex();
			units.pop(unitIndex);
			tileMatrix.getCell(x, y).removeUnit();
			// unit indexs are off by one on tiles so all above index must be modified //
			for (int i = 0; i < tileMatrix.getHeight(); i++) {
				for (int j = 0; j < tileMatrix.getWidth(); j++) {
					Tile currentTile = tileMatrix.getCell(j, i);
					if (currentTile.containsUnit()) {
						int index = currentTile.getUnitIndex();
						if (index >= unitIndex) {
							currentTile.setUnit(index - 1);
							tileMatrix.setCell(j, i, currentTile);
						}
					}
				}
			}
		}
		else {
			std::cerr << "\nERROR: CANNOT REMOVE UNIT";
		}
		
	}
	void moveCursor(int x, int y) {
		// moves cursors position and updates map for tile texture
		if (previousCursorPosition.first != x || previousCursorPosition.second != y) {
			Tile tile = tileMatrix.getCell(previousCursorPosition.first, previousCursorPosition.second);
			tileMatrix.getCell(previousCursorPosition.first, previousCursorPosition.second).setTileState(tile.getPreviousTileState());
			cursorPosition.first = x;
			cursorPosition.second = y;
			previousCursorPosition.first = x;
			previousCursorPosition.second = y;
		}
		tileMatrix.getCell(x, y).setTileState(1);
		displayWorld();
	}
	void clearTileStates() {
		// reverts all tile states to normal // 
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				tileMatrix.getCell(j, i).setTileState(0);
			}
		}
		unitSelected = false;
		moveCursor(cursorPosition.first, cursorPosition.second); // sets mouse cursor back
	}
	void addEdge(int src, int dst, int weight) {
		//std::cout << "\naddEdge(" << src << "," << dst << "," << weight << ")";
		adjMatrix.setCell(src,dst,weight);
	}
	void removeEdge(int src, int dst) {
		adjMatrix.setCell(src, dst, 0);
	}
	bool checkEdge(int src, int dst) {
		// Checks if edge has any weight 
		if (adjMatrix.getCell(src, dst) != 0) {
			return true;
		}
		return false;
	}
	void displayWorld() {
		// displays all tiles, tiles background change depending on their tileState
		system("cls");
		std::cout << "\nWIDTH: " << width;
		std::cout << "\nHEIGHT: " << width;
		std::cout << "\nTURNS: " << turnsTaken;
		std::cout << "\n";
		for (int i = 0; i < tileMatrix.getHeight(); i++) {
			for (int j = 0; j < tileMatrix.getWidth(); j++) {
				Tile currentTile = tileMatrix.getCell(j,i);
				if (currentTile.containsUnit()) {
					int unitType = units[currentTile.getUnitIndex()].getUnitType();
					if (units[currentTile.getUnitIndex()].isOwnedByPlayer()) {
						std::cout << friendlyUnitImagesUpperHalf[unitType];
					}
					else {
						std::cout << enemyUnitImagesUpperHalf[unitType];
					}	
				}
				else {
					displayTileUpper(currentTile); // upper and lower texters separated
				}
			}
			std::cout << "\n";
			for (int j = 0; j < tileMatrix.getWidth(); j++) {
				Tile currentTile = tileMatrix.getCell(j, i);
				if (currentTile.containsUnit()) {
					int unitType = units[currentTile.getUnitIndex()].getUnitType();
					if (units[currentTile.getUnitIndex()].isOwnedByPlayer()) {
						std::cout << friendlyUnitImagesLowerHalf[unitType];
					}
					else {
						std::cout << enemyUnitImagesLowerHalf[unitType];
					}
				}
				else {
					displayTileLower(currentTile);
				}
			}
			std::cout << "\n";	
		}
		// text information at bottom
		std::cout << "\nSELECTED: (" << cursorPosition.first << "," << cursorPosition.second << ")";
		if (tileMatrix.getCell(cursorPosition.first, cursorPosition.second).containsUnit()) {
			Unit unit = units[tileMatrix.getCell(cursorPosition.first, cursorPosition.second).getUnitIndex()];
			std::cout << "\nSELECTED UNIT TURNS: " << unit.getTurns();
			std::cout << "\nSELECTED UNIT TEAM: " << unit.getTeam();
			std::cout << "\nSELECTED UNIT HEALTH: " << unit.getHealth();
		}

	}
	void selectUnit(int x, int y) {
		// changes graph's unit, only friendly can be owned
		if (tileMatrix.getCell(x, y).containsUnit() && units[tileMatrix.getCell(x, y).getUnitIndex()].isOwnedByPlayer()) {
			selectedUnit = units[tileMatrix.getCell(x, y).getUnitIndex()];
			unitSelected = true;
		}
		else {
			unitSelected = false;
		}
	}
	void initDijkstras(int srcX, int srcY) {
		// setup for shortest path algorithm //
		for (int i = 0; i < tileMatrix.getHeight(); i++) {
			for (int j = 0; j < tileMatrix.getWidth(); j++) {
				tileMatrix.getCell(i, j).visited = false;
				if (srcX == i && srcY == j) {
					tileMatrix.getCell(i, j).shortestDistanceFromTile = 0; // identical tile
				}
				else {
					tileMatrix.getCell(i, j).shortestDistanceFromTile = INT_MAX; // other tile, start with inf
				}
				tileMatrix.getCell(i, j).previousTilePosition = std::pair<int,int>(-1,-1); // no previous tile, make -1,-1, used for walls and first case
			}
		}
	}
	void printTilesDebug() {
		// debugging only, lists all tiles and their variables // 
		for (int i = 0; i < tileMatrix.getHeight(); i++) {
			for (int j = 0; j < tileMatrix.getWidth(); j++) {
				Tile tile = tileMatrix.getCell(j, i);
				std::cout << "\n\nTile: " << tile.tileIdentifier;
				std::cout << "\nType: " << tile.getTileType();
				std::cout << "\nVisited?: " << tile.visited;
				std::cout << "\nPrevious Tile Pos: " << "(" << tile.previousTilePosition.first << "," << tile.previousTilePosition.second << ")";
				std::cout << "\nTile Distance from Source: " << tile.shortestDistanceFromTile;

			}
		}
	}
	void Dijkstra(int x1, int y1) {
		// uses Dijkstra’s Shortest Path Algorithm to find the shortest path from two coordinates for the tile matrix // 
		initDijkstras(x1,y1);
		PriorityQueue<Tile> pq(size);
		Tile currentTile = tileMatrix.getCell(x1, y1);
		bool finished = false;
		while (!finished) {
			currentTile.visited = true;
			tileMatrix.setCell(currentTile.getPosition().first, currentTile.getPosition().second,currentTile);
			for (int i = 0; i < tileMatrix.getHeight(); i++) {
				for (int j = 0; j < tileMatrix.getWidth(); j++) {
					Tile comparisonTile = tileMatrix.getCell(j, i);
					if (!comparisonTile.visited && checkEdge(currentTile.tileIdentifier, comparisonTile.tileIdentifier)) {
						pq.push(comparisonTile, comparisonTile.shortestDistanceFromTile);
					}
				}
			}
			//std::cout << "\nQ: ";
			//std::cout << currentTile.tileIdentifier << "|, ";
			while (!pq.isEmpty()) {
				Tile tile = pq.pop();
				//std::cout << tile.tileIdentifier << ", ";
				int distance = currentTile.shortestDistanceFromTile + adjMatrix.getCell(currentTile.tileIdentifier, tile.tileIdentifier);
				if (tile.shortestDistanceFromTile > distance) {
					tile.shortestDistanceFromTile = distance;
					tile.previousTilePosition = currentTile.getPosition();
				}
				tileMatrix.setCell(tile.getPosition().first, tile.getPosition().second, tile);
			}
			finished = true;
			PriorityQueue<Tile> pq2(size); // from all unVisited tiles, the one with the shortest distance will be at the top for next visit
			for (int i = 0; i < tileMatrix.getHeight(); i++) {
				for (int j = 0; j < tileMatrix.getWidth(); j++) {
					Tile comparisonTile = tileMatrix.getCell(j, i);
					if (!comparisonTile.visited && comparisonTile.getTileType() != 1) {
						pq2.push(comparisonTile, comparisonTile.shortestDistanceFromTile);
					}
				}
			}
			if (!pq2.isEmpty()) {
				// if the queue was not given any data, we have visited everthing
				finished = false;
				currentTile = pq2.pop();
			}
			//printTilesDebug();
		}
		//printTilesDebug();
	}
	void nextTurn() {
		// ai movement and attack // 
		for (int i = 0; i < units.get_size(); i++) {
			if (!units[i].isOwnedByPlayer() && units[i].getHealth() > 0) {
				Dijkstra(units[i].getPosition().first,units[i].getPosition().second);
				PriorityQueue<Unit> pq(size);
				for (int j = 0; j < units.get_size(); j++) {
					if (units[j].isOwnedByPlayer() && units[j].getHealth() > 0) {
						pq.push(units[j], tileMatrix.getCell(units[j].getPosition().first, units[j].getPosition().second).shortestDistanceFromTile);
					}
				}
				// closest unit has highest priority for ALL units
				Unit toAttack = pq.pop();
				moveUnit(units[i].getPosition().first,units[i].getPosition().second, toAttack.getPosition().first, toAttack.getPosition().second);
			}
		}
		// turn cleaning
		turnsTaken++;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Tile tile = tileMatrix.getCell(j,i);
				if (tile.getTileType() == 4 && tile.containsUnit()) {
					if (units[tile.getUnitIndex()].isOwnedByPlayer()) { // win condition, have unit in secret passage for one turn // 
						winScreen();
					}
				}
				if (tile.containsUnit()) {
					units[tile.getUnitIndex()].setTurns(4); 
					if (units[tile.getUnitIndex()].getHealth() < 100) {
						units[tile.getUnitIndex()].applyDamage(-10); // damages by -10 for healing per turn;
					}	
				}
			}
		}
		clearTileStates();
		std::cout << "\n NEW TURN ! ";
	}
	Tile getPreviousTile(Tile tile) {
		// for shortest path gets the previous tile
		return tileMatrix.getCell(tile.previousTilePosition.first, tile.previousTilePosition.second);
	}
	void moveUnit(int x1, int y1, int x2, int y2) {
		// moves unit from one position to another if possible // 
		Tile sourceTile = tileMatrix.getCell(x1, y1);
		Tile destinationTile = tileMatrix.getCell(x2, y2);
		if (sourceTile.containsUnit() && destinationTile.getTileType() != 1 && selectedUnit.getTurns() > 0 && sourceTile.getPosition() != destinationTile.getPosition()) {
			Unit unit = units[sourceTile.getUnitIndex()];
			Dijkstra(x1, y1); // setup tiles from source
			Vector<Tile> stack;
			stack.push_front(destinationTile);
			while (stack.peek_front().previousTilePosition != sourceTile.getPosition()) { // Establish route until were one tile above source
				destinationTile = getPreviousTile(destinationTile);
				stack.push_front(destinationTile);
			}
			// stack is filled with tiles to visit, empty stack of all tiles we can visit //
			int moves = unit.getTurns();
			while (moves > 0 && !stack.empty()) {
				destinationTile = stack.pop_front();
				Tile previousTile = getPreviousTile(destinationTile);
				moves -= adjMatrix.getCell(previousTile.tileIdentifier, destinationTile.tileIdentifier);
			}
			units[sourceTile.getUnitIndex()].setTurns(moves);
			if (destinationTile.getPosition() != sourceTile.getPosition()) {
				// check and make sure we actually moved
				if (destinationTile.containsUnit()) {
					if (units[destinationTile.getUnitIndex()].getTeam() != units[sourceTile.getUnitIndex()].getTeam()) { // if teams are different attacks can happen
						impulseAttack(sourceTile, destinationTile);
					}
					destinationTile = getPreviousTile(destinationTile);
				}
				destinationTile.setUnit(sourceTile.getUnitIndex());
				if (destinationTile.getPosition() != sourceTile.getPosition()) {
					sourceTile.removeUnit();
				}
				tileMatrix.setCell(destinationTile.getPosition().first, destinationTile.getPosition().second, destinationTile);
				tileMatrix.setCell(sourceTile.getPosition().first, sourceTile.getPosition().second, sourceTile);
				units[destinationTile.getUnitIndex()].setPosition(destinationTile.getPosition().first, destinationTile.getPosition().second);
				clearTileStates();
			}
		}
		else {
			std::cerr << "\nERROR: NO UNIT ON TILE";
		}
	}
	void showMovement(int x, int y) {
		// on selected unit(tile) shows how far they can move // 
		Tile sourceTile = tileMatrix.getCell(x,y);
		if (tileMatrix.getCell(x, y).containsUnit() && units[sourceTile.getUnitIndex()].isOwnedByPlayer()) {
			Unit unit = units[sourceTile.getUnitIndex()];
			if (unit.getTurns() > 0) { // unit has turns no point
				Dijkstra(x, y);
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						int turnsNeeded = tileMatrix.getCell(j, i).shortestDistanceFromTile;
						if (turnsNeeded <= unit.getTurns()) {
							tileMatrix.getCell(j, i).setTileState(2);
						}
					}
				}
				displayWorld();
			}
		}
	}
	void impulseAttack(Tile first, Tile second) {
		// attack unit on second tile
		Unit defender = units[second.getUnitIndex()];
		Unit attacker = units[first.getUnitIndex()];
		defender.applyDamage(attacker.getDamage());
		if (defender.getHealth() <= 0) {
			//second.removeUnit();
			//tileMatrix.setCell(second.getPosition().first,second.getPosition().second,second);
			removeUnit(second.getPosition().first, second.getPosition().second); // unit died
		}
		else {
			units[second.getUnitIndex()].setHealth(defender.getHealth()); // not dead, apply damage
		}
	}
	void winScreen() {
		// displays win message and ends game
		system("cls");
		std::cout << "\nVICTORY";
		exit(0);
		system("pause");
	}
};
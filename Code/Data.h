#pragma once
#include <tuple>
/**
* Data.h
* Stores individual compenents as classes needed for game function
*
* Includes map terrain tiles, units, structures
*
* - TILE TYPES - *
* 0 = NO TILE
* 1 = Wall
* 2 = Empty Tile
* 3 = Dirt
* 4 = Secret Passage
* - UNIT TYPES - *
* 0 = Blank Unit (unable to do anything)
* 1 = Footman
**/

extern const int unitHealths[]{
	0, // NO UNIT
	100, // Footman
};

extern const std::string tileImagesUpperHalf[] = {
	"\033[37;105m???\033[m", // NO TILE
	"\033[30;47m o \033[m", // Agent
	"\033[30;47m x \033[m", // Enemy
	"\033[30;45m###\033[m", // Wall
	"\033[37;43m///\033[m", // Empty Tile
	"\033[30;42m***\033[m", // Dirt
	"\033[34;105m|v|\033[m", // Secret Passage
};

extern const std::string tileImagesLowerHalf[] = {
	"\033[37;105m???\033[m", // NO TILE
	"\033[30;47m/D\\\033[m", // Agent
	"\033[30;47m/D\\\033[m", // Enemy
	"\033[30;45m###\033[m", // Wall
	"\033[37;43m///\033[m", // Empty Tile
	"\033[30;42m***\033[m", // Dirt
	"\033[34;105m|^|\033[m", // Secret Passage
};

extern const std::string friendlyUnitImagesUpperHalf[] = {
	"\033[37;105m???\033[m", // NO TILE
	"\033[30;47m o \033[m", // Footman
};

extern const std::string friendlyUnitImagesLowerHalf[] = {
	"\033[37;105m???\033[m", // NO TILE
	"\033[30;47m/D\\\033[m", // Footman
};

extern const std::string enemyUnitImagesUpperHalf[] = {
	"\033[37;105m???\033[m", // NO TILE
	"\033[30;41m o \033[m", // Footman
};

extern const std::string enemyUnitImagesLowerHalf[] = {
	"\033[37;105m???\033[m", // NO TILE
	"\033[30;41m/D\\\033[m", // Footman
};

class Unit {
private:
	int posX;
	int posY;
	int health;
	int team; // 0 is no team, 1 is player team, 2+ is non-player team
	int unitType;
	int turns = 4;
	int damage = 50;
	bool isSelected = false;
public:
	Unit(int unitType, int team) {
		this->unitType = unitType;
		posX = 0;
		posY = 0;
		health = unitHealths[unitType];
		this->team = team;
	}
	Unit() {
		unitType = 0;
		posX = 0;
		posY = 0;
		health = 0;
		team = 0;
	}
	void setPosition(int x, int y) {
		posX = x;
		posY = y;
	}
	std::pair<int, int> getPosition() {
		return std::pair<int, int>(posX, posY);
	}
	bool operator==(const Unit& other) {
		if (unitType == other.unitType) {
			return true;
		}
		return false;
	}
	void operator=(const Unit& other) {
		posX = other.posX;
		posY = other.posY;
		unitType = other.unitType;
		health = other.health;
		team = other.team;
		turns = other.turns;
		isSelected = other.isSelected;
	}
	int getUnitType() { return unitType; }
	int getHealth() { return health; }
	int getTeam() { return team; }
	bool isOwnedByPlayer() {
		if (team == 1) {
			return true;
		}
		return false;
	}
	void setTurns(int turns) {
		this->turns = turns;
	}
	int getTurns() { return turns; }
	void applyDamage(int dmg) {
		health -= dmg;
	}
	void setHealth(int hp) {
		health = hp;
	}
	int getDamage() { return damage;}
};

class Tile {
private:
	int posX = 0;
	int posY = 0;
	int tileType;
	int health;
	bool unitOnTile = false;
	int unitIndex = -1; // stores index to unit Vector so we know what unit is on tile, -1 means no unit
	int tileState = 0; // for texturing, selected, movement zone, attack zone
	int previousTileState = 0; // texturing reversion upon multiple changes (cursor on movement tile)
public:
	// shortest path stuff //
	bool visited = false; // used for Dijkstra’s Shortest Path Algorithm
	std::pair<int, int> previousTilePosition;
	int shortestDistanceFromTile = INT_MAX;
	int tileIdentifier;
	Tile(int tileType) {
		health = 0;
		this->tileType = tileType;
	}
	Tile() {
		tileType = 0;
		health = 0;
	}
	~Tile() {
		
	}
	void setPosition(int x, int y) {
		posX = x;
		posY = y;
	}
	void setTileState(int state) {
		previousTileState = tileState;
		tileState = state;
	}
	void setPreviousTileState(int state) {
		previousTileState = state;
	}
	std::pair<int, int> getPosition() {
		return std::pair<int, int>(posX, posY);
	}
	bool operator==(const Tile& other) {
		if (tileType == other.tileType) {
			return true;
		}
		return false;
	}
	bool operator!=(const Tile& other) {
		if (tileType != other.tileType) {
			return true;
		}
		return false;
	}
	void operator=(const Tile& other) {
		posX = other.posX;
		posY = other.posY;
		tileType = other.tileType;
		health = other.health;
		unitOnTile = other.unitOnTile;
		unitIndex = other.unitIndex;
		tileState = other.tileState;
		visited = other.visited;
		previousTilePosition = other.previousTilePosition;
		tileIdentifier = other.tileIdentifier;
		shortestDistanceFromTile = other.shortestDistanceFromTile;
		previousTileState = 0;
	}
	int getTileType() {
		return tileType;
	}
	void setUnit(int unitIndex) {
		this->unitIndex = unitIndex;
		unitOnTile = true;
	}
	void removeUnit() {
		unitIndex = -1;
		unitOnTile = false;
	}
	bool containsUnit() { return unitOnTile; }
	int getUnitIndex() { return unitIndex; }
	int getTileState() { return tileState; }
	int getPreviousTileState() { return previousTileState; }
	std::pair<int, int> getPreviousTilePosition() { return previousTilePosition; }
};


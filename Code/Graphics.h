/**
* Graphics.h
* Contains all the data for graphical display
**/
#pragma once
#define ESC "\033["
#define RESET "\033[m"
// TEXT COLORS //
#define BLACK_TXT "30;"
#define RED_TXT "31;"
#define GREEN_TXT "32;"
#define YELLOW_TXT "33;"
#define BLUE_TXT "34;"
#define PURPLE_TXT "35;"
#define CYAN_TXT "36;"
#define WHITE_TXT "37;"
// BACKGROUNDS //
#define BLACK_BKG "40m"
#define RED_BKG "41m"
#define GREEN_BKG "42m"
#define YELLOW_BKG "43m"
#define BLUE_BKG "44m"
#define PURPLE_BKG "45m"
#define CYAN_BKG "46m"
#define WHITE_BKG "47m"

std::string tileStateTextures[]{
	"", // default
	WHITE_BKG, // selected
	BLUE_BKG, // movement
	RED_BKG, // attack
};
std::string tileBackgroundColorDefault[]{
	PURPLE_BKG, // NO TILE
	BLACK_BKG, // WALL
	YELLOW_BKG, // EMPTY
	GREEN_BKG, // DIRT
	PURPLE_BKG, // SECRET PASSAGE
};
std::string tileTextColorDefault[]{
	WHITE_TXT, // NO TILE
	WHITE_TXT, // WALL
	CYAN_TXT, // EMPTY
	BLACK_TXT, // DIRT
	BLUE_TXT, // SECRET PASSAGE
};
std::string tileTextureUpper[]{
	"???", // NO TILE
	"%##", // WALL
	"/|/", // EMPTY
	"*~*", // DIRT
	"[v]", // SECRET PASSAGE
};
std::string tileTextureLower[]{
	"???", // NO TILE
	"#%#", // WALL
	"|//", // EMPTY
	"**~", // DIRT
	"[^]", // SECRET PASSAGE
};
extern void displayTileUpper(Tile tile) {
	int type = tile.getTileType();
	if (tile.getTileState() == 0) {
		std::cout << ESC << tileTextColorDefault[type] << tileBackgroundColorDefault[type]; // default background
	}
	else {
		int tileState = tile.getTileState();
		std::cout << ESC << tileTextColorDefault[type] << tileStateTextures[tileState];
	}
	std::cout << tileTextureUpper[type] << RESET;
}
extern void displayTileLower(Tile tile) {
	int type = tile.getTileType();
	if (tile.getTileState() == 0) {
		std::cout << ESC << tileTextColorDefault[type] << tileBackgroundColorDefault[type]; // default background
	}
	else {
		int tileState = tile.getTileState();
		std::cout << ESC << tileTextColorDefault[type] << tileStateTextures[tileState];
	}
	std::cout << tileTextureLower[type] << RESET;
}
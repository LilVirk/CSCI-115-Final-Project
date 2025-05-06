#pragma once
#include <fstream>
#include <sstream>
#include "Vector.h"
#include <iostream>
#include "GameGraph.h"
#include "Matrix.h"
/**
* 
* Creates a MapGraph from imported text file
* Places Units as well, read How To Make Map for more information
* 
**/
MapGraph processMapFile(std::string fileContents) {
	// Takes a string and process it into a MapGraph and returns it //
	int fileSize = fileContents.size();
	int mapWidth = 0;
	int mapHeight = 0;
	std::string mapName;
	int i = 0;
	while (i < fileSize && fileContents[i] != '{') {
		if (fileContents[i] == 'N') {
			i += 2;
			while (fileContents[i] != '\n') {
				mapName += fileContents[i];
				i++;
			}
		}
		if (fileContents[i] == 'W') {
			std::string widthValue;
			i += 2;
			while (fileContents[i] != '\n') {
				widthValue += fileContents[i];
				i++;
			}
			std::istringstream(widthValue) >> mapWidth;
		}
		if (fileContents[i] == 'H') {
			std::string widthValue;
			i += 2;
			while (fileContents[i] != '\n') {
				widthValue += fileContents[i];
				i++;
			}
			std::istringstream(widthValue) >> mapHeight;
		}
		i++;
	}
	if (i < fileSize) {
		// Map contains data, begin processing map // 
		MapGraph map(mapWidth,mapHeight);
		Vector<int> tiles; // used for creating adjMatrix // 
		i += 2;
		int fileType = 0;
		// READ MAP DATA // 
		int tileIdentifier = 0;
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (fileContents[i] == '0') {
					fileType = 0;
				} else if (fileContents[i] == '#') {
					fileType = 1;
				} else if (fileContents[i] == '/') {
					fileType = 2;
				} else if (fileContents[i] == '*') {
					fileType = 3;
				} else if (fileContents[i] == '@') {
					fileType = 4;
				}
				map.addTile(x,y,fileType, tileIdentifier);
				tileIdentifier++;
				tiles.push_back(fileType);
				i++;
			}
			i++;
		}
		// CREATE ADJ MATRIX // 
		int index = 0;
		int val = 1;
		//tiles.print_vector();
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (x > 0) { // HasLeft
					if (tiles[index-1] == 1) {
						val = 0; // Wall
					}
					else if (tiles[index-1] == 3) {
						val = 2;
					}
					else {
						val = 1;
					}
					//map.addEdge(index - 1, index,val);
					map.addEdge(index, index-1, val);
				}
				if (x < mapWidth - 1) { // HasRight
					if (tiles[index + 1] == 1) {
						val = 0; // Wall
					}
					else if (tiles[index + 1] == 3) {
						val = 2;
					}
					else {
						val = 1;
					}
					//map.addEdge(index + 1, index, val);
					map.addEdge(index, index+1, val);
				}
				if (y > 0) { // HasBottom
					if (tiles[index - mapWidth] == 1) {
						val = 0; // Wall
					}
					else if (tiles[index - mapWidth] == 3) {
						val = 2;
					}
					else {
						val = 1;
					}
					//map.addEdge(index - mapWidth, index, val);
					map.addEdge(index, index-mapWidth, val);
				}
				if (y < mapHeight - 1) { // HasTop
					if (tiles[index + mapWidth] == 1) {
						val = 0; // Wall
					}
					else if (tiles[index + mapWidth] == 3) {
						val = 2;
					}
					else {
						val = 1;
					}
					//map.addEdge(index - mapWidth, index, val);
					map.addEdge(index, index+mapWidth, val);
				}
				index++;
			}
		}
		i += 3; // Iterate to next matrix
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (fileContents[i] == '0') {
					fileType = 0;
				}
				else if (fileContents[i] == '1') {
					fileType = 1;
				}
				if (fileType != 0) {
					map.addUnit(x, y, fileType, 1); // team 1 friendly
				}
				i++;
			}
			i++;
		}
		// READ ENEMY  UNIT DATA //
		i += 3; // Iterate to next matrix
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (fileContents[i] == '0') {
					fileType = 0;
				}
				else if (fileContents[i] == '1') {
					fileType = 1;
				}
				if (fileType != 0) {
					map.addUnit(x, y, fileType, 2); // team 2 enemy
				}
				i++;
			}
			i++;
		}
		return map;
	}
	else {
		std::cerr << "\nERROR: MAP CONTAINS NO DATA";
	}
	return MapGraph();
}

MapGraph importMapFile(std::string fileName) {
	// Try multiple path options to find the file
	std::vector<std::string> pathsToTry;
	
	// Current directory
	pathsToTry.push_back(fileName);
	
	// Maps subdirectory
	pathsToTry.push_back("Maps/" + fileName);
	
	// Add extension if missing
	if (fileName.find(".txt") == std::string::npos) {
		pathsToTry.push_back(fileName + ".txt");
		pathsToTry.push_back("Maps/" + fileName + ".txt");
	}
	
	// Try each path
	for (const std::string& path : pathsToTry) {
		std::string fileContents; 
		std::ifstream file(path);
		
		if (file.is_open()) {
			std::cout << "\nLoading map from: " << path;
			std::ostringstream oss;
			oss << file.rdbuf();
			fileContents = oss.str();
			file.close();
			
			if (!fileContents.empty()) {
				return processMapFile(fileContents);
			}
		}
	}
	
	std::cerr << "\nERROR: Could not find or open map file: " << fileName;
	return MapGraph();
}
Read "How To Make A Map Tutorial" to make a map

Goal: Eliminate all enemy units (colored red) and have at least one of your units sit in a secret passage for one turn (enemies take turn before
win condition so you can die before)

Controls: 
SPACE -> Select Unit, will show in blue where unit can be moved
P -> Move selected unit, if space is outside blue move zone, unit will take shortest path to location with their turns they have
SHIFT -> New turn, resets until movement and allows for enemy AI to move as well. HP is regenerated at 10HP a turn up to 100 for all units
Q -> Quit Game
ESC -> Cancel unit selection

*At beginning of game type in file name for your map, note it must end in *.txt (Ex/ test.txt). Place txt where exe is.
*When placing units in txt don't overlap enemies and friendlies, make sure there is at least one path to player otherwise AI will crash

Files:

CreateMap.h -> Creates GameGraph from txt file
Data.h -> Holds unit and tile data and some texturing data
GameGraph.h -> Main header for game, controls graph including shortest path and links tiles, units, and player interface together including display
Graphics.h -> Holds items for texturing units and tiles, holds tile states which change color of tile depending on its purpose (movement, selection, etc)
Matrix.h -> Holds 2D array of items with functions for manipulation. Used for adjMatrix and tileMatrix, units saved as vector
PriorityQueue.h -> Queue with a min heap base, smallest value is highest priority. Used for shortest path alogorithm
Vector.h -> From scratch vector class
Source.cpp -> User control and initilization
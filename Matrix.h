/**
*
* Data structure that stores a given item as a 2D array with functions for read, write, and more
*
**/
#pragma once
#include <iostream>
#include <utility>
using namespace std;
template<typename T>
class Matrix {
private:
	T** array;
	int size;
	int width;
	int height;
public:
	Matrix() {
		array = nullptr;
		size = 0;
		width = 0;
		height = 0;
	}
	Matrix(int width, int height) {
		initMatrix(width, height);
	}
	Matrix(const Matrix& other) {
		copyMatrix(other);
	}
	~Matrix() {
		cleanMatrix();
	}
	void cleanMatrix() {
		if (array != nullptr) {
			for (int i = 0; i < width; i++) {
				delete[] array[i];
			}
			delete[] array;
		}
	}
	void operator=(const Matrix& other) {
		copyMatrix(other);
	}
	void initMatrix(int width, int height) {
		this->width = width;
		this->height = height;
		size = width * height;
		array = new T * [width];
		for (int i = 0; i < width; i++) {
			array[i] = new T[height];
		}
	}
	void copyMatrix(const Matrix& other) {
		cleanMatrix();
		width = other.width;
		height = other.height;
		int size = width * height;
		if (size > 0) {
			array = new T*[width];
			for (int i = 0; i < width; i++) {
				array[i] = new T[height];
				for (int j = 0; j < height; j++) {
					array[i][j] = other.array[i][j];
				}
			}
		}
		else {
			array = nullptr;
		}
	}
	void setCell(int x, int y, T item) {
		try {
			array[x][y] = item;
		}
		catch (exception &e){
			std::cerr << "\nERROR: UNABLE TO INPUT ITEM INTO MATRIX " << e.what();
		}
	}
	T& getCell(int x, int y) {
		try {
			return array[x][y];
		}
		catch (exception& e) {
			std::cerr << "\nERROR: UNABLE TO RETURN ITEM FROM MATRIX " << e.what();
		}
	}
	void swapCells(int x1, int y1, int x2, int y2) {
		try {
			T tempOne = array[x1][y1];
			T tempTwo = array[x2][y2];
			array[x1][y1] = tempTwo;
			array[x2][y2] = tempOne;

		}
		catch (exception& e) {
			std::cerr << "\nERROR: UNABLE TO RETURN ITEM FROM MATRIX " << e.what();
		}
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	std::pair<int, int> locateCell(T key) {
		// Finds given cell and returns its position x,y //
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (array[i][j] == key) {
					return std::pair<int, int>(i, j);
				}
			}
		}
		std::cerr << "\nERROR: ITEM NOT FOUND IN MATRIX";
		return std::pair<int, int>();
	}
	void printMatrix() {
		// Prints contents of matrix to console // 
		std::cout << "\n";
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				std::cout << array[j][i] << " ";
			}
			std::cout << "\n";
		}
	}
};
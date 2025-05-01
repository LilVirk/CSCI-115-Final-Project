/**
* PriorityQueue.h
* Makes a PriorityQueue using a min heap
* 
* Data is stored as int in a queue, beacuse of the min heap the data with the smallest value (double) 
* is at the top and thus has the highest priority
**/

#pragma once
#include <iostream>
template<typename T>
struct Data {
	T index; // value
	double value; // priority
	void operator=(const Data &other) {
		index = other.index;
		value = other.value;
	}
};
template<typename T>
class PriorityQueue {
private:
	void swap(int a, int b) {
		Data<T> tempA = queue[a];
		queue[a] = queue[b];
		queue[b] = tempA;

	}
	void MinHeapify(int i, int size) {
		int left, right, smallest;
		left = 2 * i + 1; // left child of a[i];
		right = 2 * i + 2; // right child of a[i]
		smallest = i;
		if (left < size && queue[left].value < queue[i].value) {
			smallest = left;
		}
		if (right < size && queue[right].value < queue[smallest].value) {
			smallest = right;
		}
		if (smallest != i) { // there was a swap
			swap(i, smallest);
			MinHeapify(smallest,size);
		}
	}
	int capacity;
	int numberOfElements;
	Data<T>* queue;
public:
	PriorityQueue() {
		capacity = 0;
		numberOfElements = 0;
		queue = nullptr;
	}
	PriorityQueue(int capacity) {
		this->capacity = capacity;
		numberOfElements = 0;
		queue = new Data<T>[capacity];
	}
	~PriorityQueue() {
		delete[] queue;
	}
	int getNumberElements() { return numberOfElements;}
	bool isEmpty() { return (numberOfElements == 0); }
	bool isFull() { return numberOfElements == capacity; }
	int getParent(int child) {
		if (child % 2 == 0) {
			return (child / 2) - 1;
		}
		else {
			return child / 2;
		}
	}
	int getLeftChild(int parent) { return 2 * parent + 1; }
	int getRightChild(int parent) { return 2 * parent + 2; }
	void push(T index, double value) {
		if (numberOfElements < capacity) {
			Data<T> newData;
			newData.index = index;
			newData.value = value;
			int i = numberOfElements;
			while(i != 0 && newData.value < queue[i/2].value) {
				queue[i] = queue[i / 2]; // move down
				i /= 2;
			} // move to parent
			queue[i] = newData;
			numberOfElements++;
		}
		else {
			std::cout << "ERROR: CANNOT PUSH TO HEAP, OUT OF CAPACITY\n";
		}
	}
	T pop() {
		if (numberOfElements > 0) {
			Data<T> temp = queue[0];
			queue[0] = queue[numberOfElements - 1];
			MinHeapify(0, numberOfElements);
			numberOfElements--;
			return temp.index;
		}
		else {
			Data<T> temp;
			temp.value = 0;
			//temp.index = NULL;
			return temp.index;
		}
	}
	void decreaseKey(int index, double value) {
		// decreease value for lowering/raising priority
		int i = 0;
		bool found = false;
		while (!found && i < numberOfElements) {
			if (queue[i].index == index) {
				queue[i].value = value;
				found = true;
			}
			i++;
		}
		if (found) {
			int child = i - 1;
			int parent = getParent(child);
			while (queue[child].value < queue[parent].value && child >= 0 && parent >= 0) {
				swap(child, parent);
				child = parent;
				parent = getParent(child);
			}
		}
	}
	void buildMinHeap() {
		// traverses the tree backwards, left, bottom top
		for (int i = numberOfElements / 2; i >= 0; i--) {
			MinHeapify(i,numberOfElements);
		}
	}
	void display() {
		std::cout << "Priority Queue\n";
		std::cout << "\t capacity" << capacity << "\n";
		std::cout << "\t #Elements" << numberOfElements << "\n";
		for (int i = 0; i < numberOfElements; i++) {
			std::cout << i << ": (" << queue[i].index << "," << queue[i].value << ")\n";
		}
		std::cout << "\n";
	}
	double getFrontValue() {
		return queue[0].value;
	}
};
#include "../include/Box.hpp"
#include <cstdlib>

using namespace std;

// Box default constructor
Box :: Box() {
	setN(10); // The dimensions of simulation-box
	setSize(getN() + 2); // The dimensions of the simulation-box with borders
	// Allocate memory for densities and x-values
	x = new float(getSize() * getSize());
	densities = new float(getSize() * getSize());
}

// Box destructor
Box :: ~Box() {
	// Free memory
	delete(x);
	delete(densities);
}

void Box :: setN(const int N) {
	this->N = N;
}

void Box :: setSize(const int size) {
	this->size = size;
}

void Box :: setXAtIndex(const int x, const int y, const float value) {
	this->x[y * getSize() + x] = value;
}

void Box :: setDensity(const int x, const int y, const float value) {
	this->densities[y * getSize() + x] = value;
}

int Box :: getN() const {
	return this->N;
}

int Box :: getSize() const {
	return this->size;
}

float Box :: getXAtIndex(const int x, const int y) {
	return this->x[y * getSize() + x];
}

float Box :: getDensity(const int x, const int y) {
	return this->densities[y * getSize() + x];
}

void Box :: addDensities(const float dt) {
	// Add densities to array
	for (int i = 0; i < getSize(); i++) {
		x[i] += dt * densities[i];
	}
}

std::vector<sf::RectangleShape> Box :: renderRectangles (int width, int height) {
	// Return a vector of rectangles rendered from the box
	vector<sf::RectangleShape> rects;
	sf::RectangleShape temp(sf::Vector2f(0,0));
	rects.push_back(temp);
	return rects;
}

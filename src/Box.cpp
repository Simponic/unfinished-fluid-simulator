#include "../include/Box.hpp"
#include <bits/stdc++.h>
#include <cstdlib>

using namespace std;

// Box default constructor
Box :: Box() {
	setN(10); // The dimensions of simulation-box
	setSize(getN() + 2); // The dimensions of the simulation-box with borders
	// Allocate memory for densities and x-values
	x = new float[getSize() * getSize()];
	densities = new float[getSize() * getSize()];
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

void Box :: setVelocityAtIndex(const int x, const int y, const float value) {
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

float Box :: getVelocityAtIndex(const int x, const int y) {
	return this->x[y * getSize() + x];
}

float Box :: getDensity(const int x, const int y) {
	return this->densities[y * getSize() + x];
}

void Box :: setBounds() {
	return;
}

void Box :: diffuse_bad(const float dt, const float diff) {
	// Original array buffer
	//float* x0 = new float(getSize() * getSize());
	float x0[getSize() * getSize()] = {};
	// Copy array to buffer
	memcpy(x0, x, sizeof(float) * getSize() * getSize());


	// Calulcate the diffusion rate
	float a = dt * diff * this->N * this->N;
	
	for (int i = 1; i <= this->N; i++) {
		for (int j = 1; j <= this->N; j++) {
			int y = j * getSize();
			this->x[y + i] = x0[y + i] + a * (
					x0[y + (i - 1)] +
					x0[y + (i + 1)] +
					x0[(j - 1) * getSize() + i] + 
					x0[(j + 1) * getSize() + i] -
					4*x0[y + i]
				);
		}
	}
} 

void Box :: addDensities(const float dt) {
	// Add densities to array
	for (int i = 0; i < getSize(); i++) {
		x[i] += dt * densities[i];
	}
}

std::vector<sf::RectangleShape> Box :: renderRectangles (const int width, const int height) {
	// Return a vector of rectangles rendered from the box
	vector<sf::RectangleShape> rects;

	sf::RectangleShape temp(sf::Vector2f(0,0));
	temp.setSize(sf::Vector2f(width - 2, height -2 ));

	// Draw density tiles
	for (int i = 0; i < getSize(); i++) {
		for (int j = 0; j < getSize(); j++) {
			cout << getVelocityAtIndex(i, j) << endl;
			sf::Color color(50 + (int)getVelocityAtIndex(i,j),
					50 + (int)getVelocityAtIndex(i,j),
					50 + (int)getVelocityAtIndex(i,j)
				);
			temp.setPosition(sf::Vector2f(i * width, j * height));
			temp.setFillColor(color);
			rects.push_back(temp);
		}
	}

	return rects;
}


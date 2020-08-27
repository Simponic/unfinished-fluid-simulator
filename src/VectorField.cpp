#include <SFML/Graphics.hpp>
#include "../include/VectorField.hpp"

using namespace std;

VectorField :: VectorField() : width(10), height(10) {
	this->initializeVectors();
}

VectorField :: VectorField(const int width, const int height) : width(width), height(height) {
	this->initializeVectors();
}

VectorField :: ~VectorField() {}

void VectorField :: initializeVectors() {
	// Initialize vector fields to 0
	/*
	for (int i = 0; i < this->width; i++) {
		vector<float> temp;
		for (int j = 0; j < this->height; j++) {
			temp.push_back(0.0f);
		}
		VectorXValues.push_back(temp);
		VectorYValues.push_back(temp);
	}
	*/
	vector<float> zeroFilledVector(this->width, 0.0f);
	vector< vector<float> > initializedValues(this->height, zeroFilledVector);

	// Use = operator to do deep copy
	this->VectorXValues = initializedValues;
	this->VectorYValues = initializedValues;
}

void VectorField :: setXComponentAtIndex(const int x, const int y, const float value) {
	// Set x-component of vector at index
	this->VectorXValues[x][y] = value;
}

float VectorField :: getXComponentAtIndex(const int x, const int y) {
	// Return the value of x-component of vector at an index
	return this->VectorXValues[x][y];
}

void VectorField :: setYComponentAtIndex(const int x, const int y, const float value) {
	// Set y-component of vector at index
	this->VectorYValues[x][y] = value;
}

float VectorField :: getYComponentAtIndex(const int x, const int y) {
	// Return the value of y-component of vector at an index
	return this->VectorYValues[x][y];
}

int VectorField :: getWidth() {
	// Return width of vector field
	return this->width;
}

int VectorField :: getHeight() {
	// Return height of vector field
	return this->height;
}

void VectorField :: renderVectorField(sf::RenderWindow &window, const float tileWidth) {
	// Render the vector field onto an SFML Window
	float angle, length, Ux, Uy;
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			// Calculate magnitude and direction of vector
			Ux = this->getXComponentAtIndex(i, j);
			Uy = this->getYComponentAtIndex(i, j);
			angle = atan2(Uy, Ux);
			length = sqrtf(Ux * Ux + Uy * Uy);

			// Draw a line (thin rectangle rotated with width 3px)
			sf::RectangleShape line(sf::Vector2f(length * 5, 1.0f));
			line.rotate(angle * 180 / PI);

			// Set position to be in the center of each tile
			line.setPosition(i * tileWidth + 0.5f * tileWidth, j * tileWidth + 0.5f * tileWidth);

			window.draw(line);
		}
	}
}

void VectorField :: addVectorSource(VectorField *b, const float dt) {
	if (this->width != b->getWidth() || this->height != b->getHeight()) {
		// Make sure dimensions of vector fields are the same
		return;
	}
	float x1, x2, y1, y2;
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			x1 = this->getXComponentAtIndex(i, j);
			x2 = b->getXComponentAtIndex(i, j);
			y1 = this->getYComponentAtIndex(i, j);
			y2 = b->getYComponentAtIndex(i, j);

			// Add the vector source and apply timestep
			this->setXComponentAtIndex(i, j, x1 + dt * x2);
			this->setYComponentAtIndex(i, j, y1 + dt * y2);
			b->setXComponentAtIndex(i, j, x2 - dt * x2);
			b->setYComponentAtIndex(i, j, y2 - dt * y2);
		}
	}
}

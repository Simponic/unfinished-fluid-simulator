#include <SFML/Graphics.hpp>
#include "../include/VectorField.hpp"

using namespace std;

VectorField :: VectorField() : width(10), height(10) {
	this->initializeVectors();
}

VectorField :: VectorField(const int width, const int height) : width(width), height(height) {
	this->initializeVectors();
}

VectorField :: VectorField(VectorField &copy) {
	// Copy constructor
	this->width = copy.getWidth();
	this->height = copy.getHeight();
	this->VectorXValues = copy.getXComponents();
	this->VectorYValues = copy.getYComponents();
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

vector< vector<float> > VectorField :: getXComponents() {
	// Return the matrix of x-components
	return this->VectorXValues;
}

vector< vector<float> > VectorField :: getYComponents() {
	// Return the matrix of y-components
	return this->VectorYValues;
}

int VectorField :: getWidth() {
	// Return width of vector field
	return this->width;
}

int VectorField :: getHeight() {
	// Return height of vector field
	return this->height;
}

void VectorField :: setXBoundaries() {
	// Solve the boundary conditions for the x component of the vector field

	for (int i = 1; i < this->height - 2; i++) {
		this->setXComponentAtIndex(i, 0, this->getXComponentAtIndex(i, 1));
		this->setXComponentAtIndex(i, this->width - 1, this->getXComponentAtIndex(i, this->width - 2));
	}
	for (int j = 1; j < this->width - 2; j++) {
		this->setXComponentAtIndex(0, j, -this->getXComponentAtIndex(1, j));
		this->setXComponentAtIndex(this->height - 1, j, -this->getXComponentAtIndex(this->height - 2, j));
	}

	// Set top left corner to the average of its two neighbors
    this->setXComponentAtIndex(0, 0,
        (0.5f * (
                    this->VectorXValues[0][1]
                    + this->VectorXValues[1][0]
                )
        )
    );
    // Set top right corner to the average of its two neighbors
    this->setXComponentAtIndex(0, this->width - 1,
        (0.5f * (
                    this->VectorXValues[0][this->width - 2]
                    + this->VectorXValues[1][this->width - 1]
                )
        )
    );
    // Set bottom left corner to the average of its two neighbors
    this->setXComponentAtIndex(this->height - 1, 0,
        (0.5f * (
                    this->VectorXValues[this->height - 2][0]
                    + this->VectorXValues[this->height - 1][1]
                )
        )
    );
    // Set bottom right corner to the average of its two neighbors
    this->setXComponentAtIndex(this->height - 1, this->width - 1,
        (0.5f * (
                    this->VectorXValues[this->height - 1][this->width - 2]
                    + this->VectorXValues[this->height - 2][this->width - 1]
                )
        )
    );
}

void VectorField :: setYBoundaries() {
	// Solve the boundary conditions for the y component of the vector field

	for (int i = 1; i < this->height - 2; i++) {
		this->setXComponentAtIndex(i, 0, -this->getYComponentAtIndex(i, 1));
		this->setXComponentAtIndex(i, this->width - 1, -this->getYComponentAtIndex(i, this->width - 2));
	}
	for (int j = 1; j < this->width - 2; j++) {
		this->setXComponentAtIndex(0, j, this->getXComponentAtIndex(1, j));
		this->setXComponentAtIndex(this->height - 1, j, this->getXComponentAtIndex(this->height - 2, j));
	}

	// Set top left corner to the average of its two neighbors
    this->setYComponentAtIndex(0, 0,
        (0.5f * (
                    this->VectorYValues[0][1]
                    + this->VectorYValues[1][0]
                )
        )
    );
    // Set top right corner to the average of its two neighbors
    this->setYComponentAtIndex(0, this->width - 1,
        (0.5f * (
                    this->VectorYValues[0][this->width - 2]
                    + this->VectorYValues[1][this->width - 1]
                )
        )
    );
    // Set bottom left corner to the average of its two neighbors
    this->setYComponentAtIndex(this->height - 1, 0,
        (0.5f * (
                    this->VectorYValues[this->height - 2][0]
                    + this->VectorYValues[this->height - 1][1]
                )
        )
    );
    // Set bottom right corner to the average of its two neighbors
    this->setYComponentAtIndex(this->height - 1, this->width - 1,
        (0.5f * (
                    this->VectorYValues[this->height - 1][this->width - 2]
                    + this->VectorYValues[this->height - 2][this->width - 1]
                )
        )
    );
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

VectorField* VectorField :: diffuse(const float diffusionK, const float dt, const int iterations) {
	// Diffuse a vector field

	VectorField* diffused = new VectorField(this->width, this->height);

	float c = (this->width - 2) * (this->height - 2) * (diffusionK / 100000.0f) * dt;
	float xVel, yVel;

	for (int iters = 0; iters < iterations; iters++) {
		for (int i = 1; i < this->width - 1; i++) {
			for (int j = 1; j < this->width - 1; j++) {
				// Diffuse each component with its four neighbors
				xVel = ((this->getXComponentAtIndex(i, j)
							+ c * (
								this->getXComponentAtIndex(i - 1, j)
								+ this->getXComponentAtIndex(i + 1, j)
								+ this->getXComponentAtIndex(i, j + 1)
								+ this->getXComponentAtIndex(i, j - 1)
								)
						) / (1 + 4 * c));
				yVel = ((this->getYComponentAtIndex(i, j)
							+ c * (
								this->getYComponentAtIndex(i - 1, j)
								+ this->getYComponentAtIndex(i + 1, j)
								+ this->getYComponentAtIndex(i, j + 1)
								+ this->getYComponentAtIndex(i, j - 1)
								)
						) / (1 + 4 * c));
				diffused->setXComponentAtIndex(i, j, xVel);
				diffused->setYComponentAtIndex(i, j, yVel);
			}
		}
	}

	// Set boundaries of the grid
	diffused->setXBoundaries();
	diffused->setYBoundaries();

	return diffused;
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

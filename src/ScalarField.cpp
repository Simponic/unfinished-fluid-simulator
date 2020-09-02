#include "../include/ScalarField.hpp"
#include <bits/stdc++.h>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace std;

ScalarField :: ScalarField() : width(10), height(10) {
    this->initializeScalarField();
}

ScalarField :: ScalarField(const int width, const int height) : width(width), height(height) {
    this->initializeScalarField();
}

ScalarField :: ScalarField(ScalarField &copy) {
    // Copy constructor
    this->fieldValues = copy.getFieldValues();
    this->width = copy.getWidth();
    this->height = copy.getHeight();
}

ScalarField :: ~ScalarField() {}

void ScalarField :: initializeScalarField() {
    // Initialized (0-filled) vectors
    vector<float> zeroFilledVector(this->width, 0.0f);
    // 2D-array of zeroes
    vector< vector<float> > initializedValues(this->height, zeroFilledVector);

    // Use = operator to deep copy
    this->fieldValues = initializedValues;
}

int ScalarField :: getWidth() {
    // Return width of scalar field
    return this->width;
}

int ScalarField :: getHeight() {
    // Return height of scalar field
    return this->height;
}

vector< vector<float> > ScalarField :: getFieldValues() {
	// Return matrix of values of the scalar field
	return this->fieldValues;
}

void ScalarField :: setValueAtIndex(const int x, const int y, const float value) {
    // Set scalar field's value at an index without [] operator overloading
    this->fieldValues[x][y] = value;
}

float ScalarField :: getValueAtIndex(const int x, const int y) {
    // Return value of scalar field at index
    return this->fieldValues.at(x).at(y);
}

void ScalarField :: setBoundaries() {
	// Solve the boundary conditions of the fluid in the box

	// Set the first and last column's data
	for (int i = 1; i < this->height - 2; i++) {
		this->setValueAtIndex(i, 0, this->getValueAtIndex(i, 1));
		this->setValueAtIndex(i, this->width - 1, this->getValueAtIndex(i, this->width - 2));
	}

	// Set the first and last row's data
	for (int j = 1; j < this->width - 2; j++) {
		this->setValueAtIndex(0, j, this->getValueAtIndex(1, j));
		this->setValueAtIndex(this->height - 1, j, this->getValueAtIndex(this->height - 2, j));
	}

    // Set top left corner to the average of its two neighbors
    this->setValueAtIndex(0, 0,
        (0.5f * (
                    this->fieldValues[0][1]
                    + this->fieldValues[1][0]
                )
        )
    );
    // Set top right corner to the average of its two neighbors
    this->setValueAtIndex(0, this->width - 1,
        (0.5f * (
                    this->fieldValues[0][this->width - 2]
                    + this->fieldValues[1][this->width - 1]
                )
        )
    );
    // Set bottom left corner to the average of its two neighbors
    this->setValueAtIndex(this->height - 1, 0,
        (0.5f * (
                    this->fieldValues[this->height - 2][0]
                    + this->fieldValues[this->height - 1][1]
                )
        )
    );
    // Set bottom right corner to the average of its two neighbors
    this->setValueAtIndex(this->height - 1, this->width - 1,
        (0.5f * (
                    this->fieldValues[this->height - 1][this->width - 2]
                    + this->fieldValues[this->height - 2][this->width - 1]
                )
        )
    );
}

ScalarField* ScalarField :: diffuseScalarField(const float diffusionK, const float dt, const int iterations) {
  // Diffusion of the scalar field

    ScalarField* diffused = new ScalarField(this->width, this->height);

    float c = (this->width - 2) * (this->height - 2) * (diffusionK / 100000.0f) * dt;
	float value = 0.0f;

    for (int iters = 0; iters < iterations; iters++) {
        for (int i = 1; i < this->width - 2; i++) {
            for (int j = 1; j < this->height - 2; j++) {
              // Diffuse each cell with its 4 neighbors
              value = ((this->getValueAtIndex(i, j)
                + c * (
                  this->getValueAtIndex(i-1, j)
                  + this->getValueAtIndex(i+1, j)
                  + this->getValueAtIndex(i, j-1)
                  + this->getValueAtIndex(i, j+1)
                )
              ) / (1 + 4 * c));
              diffused->setValueAtIndex(i, j, value);
            }
        }
    }

	// Set the value of the walls of the grid
	diffused->setBoundaries();

	return diffused;
}

ScalarField* ScalarField :: advectScalarField(const float dt, VectorField *vel) {
    // Advect a scalar field, assuming it is a square
    // Source for advection algorithm: github.com/antoinefournier/Eulerian-Fluid-Simulation

    // The scalarfield pointer to return
    ScalarField* advected = new ScalarField(vel->getWidth(), vel->getHeight());

    int i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0, value;

    dt0 = (this->width) * dt;

	for (int i = 1; i < this->height - 2; i++) {
		for (int j = 1; j < this->height - 2; j++) {
			// Find the previous position of the particle
			x = i - dt0 * vel->getXComponentAtIndex(i, j);
			y = j - dt0 * vel->getYComponentAtIndex(i, j);

            // Make sure we are in bounds
			x = (x < 0.5 ? 0.5 : x);
			x = (x > (this->width + 0.5) ? (this->width + 0.5) : x);
			y = (y < 0.5 ? 0.5 : y);
			y = (y > (this->width + 0.5) ? (this->width + 0.5) : y);

            // Position of closest 4 cells
			i0 = (int)x;
			i1 = i0 + 1;
			j0 = (int)y;
			j1 = j0 + 1;

            // Ratio of linear interpolation
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;

            // New density value
            value = s0 * (
                t0 * this->getValueAtIndex(i0, j0)
                + t1 * this->getValueAtIndex(i0, j1)
                + s1 * (
                  t0 * this->getValueAtIndex(i1, j0)
                  + t1 * this->getValueAtIndex(i1, j1)
                )
            );

            advected->setValueAtIndex(i, j, value);
		}
	}
	// Set the boundaries of the advected scalar field
    advected->setBoundaries();

    return advected;
}

void ScalarField :: addScalarSource(ScalarField *b, const float dt) {
    if (this->width != b->getWidth() || this->height != b->getHeight()) {
        // Make sure that dimensions of both scalar fields are identical
        return;
    }
    float value1, value2;
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            value1 = this->getValueAtIndex(i, j);
            value2 = b->getValueAtIndex(i, j);

            // Add the scalar source
            this->setValueAtIndex(i, j, value1 + dt * value2);
            b->setValueAtIndex(i, j, value2 - value2 * dt);
        }
    }
}

void ScalarField :: renderScalarField(sf::RenderWindow &window, const float tileWidth, const sf::Color color) {
    // Render a scalar field
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            sf::RectangleShape tile(sf::Vector2f(tileWidth, tileWidth));

            // Set tile outline attributes
            tile.setOutlineThickness(1.f);
            tile.setOutlineColor(sf::Color(50, 50, 50));

            // Set tile color based on value at index
            float intensity = 10 * this->getValueAtIndex(i, j);
            tile.setFillColor(sf::Color(
                clamp(color.r + intensity, 0.0f, 255.0f),
                clamp(color.g + intensity, 0.0f, 255.0f),
                clamp(color.b + intensity, 0.0f, 255.0f)
            ));

            // Set tile position
            tile.setPosition(j * tileWidth, i * tileWidth);

            window.draw(tile);
        }
    }
}

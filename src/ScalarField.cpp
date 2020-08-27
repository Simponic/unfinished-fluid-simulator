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

void ScalarField :: setValueAtIndex(const int x, const int y, const float value) {
    // Set scalar field's value at an index without [] operator overloading
    this->fieldValues[x][y] = value;
}

float ScalarField :: getValueAtIndex(const int x, const int y) {
    // Return value of scalar field at index
    return this->fieldValues[x][y];
}

void ScalarField :: setBoundaries() {
    // Set the corners of the box to the average of their neighbors to contain
    // the scalar field within a box

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

void ScalarField :: diffuseScalarField(const float diffusionK, const float dt, const int iterations) {
    // Diffuse the scalar field through itself

    float c = (this->width - 2) * (this->height - 2) * (diffusionK / 100000.0f) * dt;

    for (int iters = 0; iters < iterations; iters++) {
        for (int i = 1; i < this->width; i++) {
            for (int j = 1; j < this->height; j++) {

            }
        }
    }
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

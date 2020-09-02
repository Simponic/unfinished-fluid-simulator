#include "../include/FluidGrid.hpp"
#include "../include/ScalarField.hpp"
#include "../include/VectorField.hpp"
#include <bits/stdc++.h>

using namespace std;

FluidGrid :: FluidGrid() : diffusionK(5.0f), gridSize(20){
    this->initializeFluidGridWithForce(0.0f, 0.0f);
}

FluidGrid :: FluidGrid(const float diffusionK, const int gridSize, const float Fx, const float Fy) : gridSize(gridSize), diffusionK(diffusionK) {
    this->initializeFluidGridWithForce(Fx, Fy);
}

FluidGrid :: ~FluidGrid() {}

void FluidGrid :: initializeFluidGridWithForce(const float Fx, const float Fy) {
    // Initialize a fluid grid, and add a force to it

    // Set number of iterations for diffusion
    this->iterations = 20;

    // Initialize fields
    this->velField = new VectorField(this->gridSize + 2, this->gridSize + 2);
    this->densityField = new ScalarField(this->gridSize + 2, this->gridSize + 2);
    this->sVelField = new VectorField(this->gridSize + 2, this->gridSize + 2);
    this->sDensityField = new ScalarField(this->gridSize + 2, this->gridSize + 2);

    // Define an area in the array where force will be added
    int applyForceAreaXmin = 0.3f * this->gridSize;
    int applyForceAreaXmax = 0.5f * this->gridSize;
    int applyForceAreaYmin = 0.3f * this->gridSize;
    int applyForceAreaYmax = 0.5f * this->gridSize;

    for (int i = applyForceAreaYmin; i <= applyForceAreaYmax; i++) {
        for (int j = applyForceAreaXmin; j <= applyForceAreaXmax; j++) {
            // Apply the force to an area
            this->velField->setXComponentAtIndex(i, j, Fx);
            this->velField->setYComponentAtIndex(i, j, Fy);
        }
    }
}

void FluidGrid :: setForceGravity(const float Fx, const float Fy) {
    // Set the magnitude of the force of gravity in each direction
    this->gravity[0] = Fx;
    this->gravity[1] = Fy;
}

void FluidGrid :: setDiffusionRate(const float diffusionK) {
    // Set the rate at which the fluid will diffuse
    this->diffusionK = diffusionK;
}

float FluidGrid :: getDiffusionRate() {
    // Get the rate at which the fluid will diffuse
    return this->diffusionK;
}

void FluidGrid :: setIterations(const int iterations) {
    // Set amount of iterations for diffusion calculation
    this->iterations = iterations;
}

int FluidGrid :: getIterations() {
    // Return amount of iterations for diffusion calculation
    return this->iterations;
}

void FluidGrid :: addDensityAndForce(const int x, const int y, const float density, const float Fx, const float Fy) {
    // Add density to a point on density field array defined at (x,y)
    // And force to a point on velocity field array defined at (x,y)

    float range = 0.10f; // Fraction of cell that is changed
    float size = (this->gridSize * range); // Size of area of cells that need to be changed
    for (int i = -(int)size; i < (int)size; i++) {
        for (int j = -(int)size; j < (int)size; j++) {
            int gridX = x + i; // X coordinate of cell that is to be changed
            int gridY = y + j; // Y coordinate of cell that is to be changed
            if (gridX < 0 || gridX > this->gridSize || gridY < 0 || gridY > this->gridSize) {
                // Make sure we are in bounds of the box
                continue;
            }

            int dx = abs(i);
            int dy = abs(j);

            float value;
            if ((dx + dy) < size) {
                // Decrement amount of density/velocity for neighbours of (x, y)
                // by (density or velocity) / (1 + change in x + change in y)
                value = this->sDensityField->getValueAtIndex(gridX, gridY) + density / (1.0f + dx + dy);
                this->sDensityField->setValueAtIndex(gridX, gridY, value);
                value = this->sVelField->getXComponentAtIndex(gridX, gridY) + Fx / (1.0f + dx + dy);
                this->sVelField->setXComponentAtIndex(gridX, gridY, value);
                value = this->sVelField->getYComponentAtIndex(gridX, gridY) + Fy / (1.0f + dx + dy);
                this->sVelField->setYComponentAtIndex(gridX, gridY, value);
            }
        }
    }
}

void FluidGrid :: updateDensityField(const float dt) {
    // Update the density field by time step dt

    // Add the new fluid
    this->densityField->addScalarSource(sDensityField, dt);

    // Diffuse the fluid
	this->densityField = this->densityField->diffuseScalarField(this->diffusionK, dt, this->iterations);

    // Advect the density field through the vector field
    this->densityField = this->densityField->advectScalarField(dt, this->sVelField);
}

void FluidGrid :: renderFluid(sf::RenderWindow &window, const sf::Color color) {
	// Render the fluid grid
    int width = window.getSize().x;
    this->densityField->renderScalarField(window, width / gridSize, color);
    this->velField->renderVectorField(window, width / gridSize);
}

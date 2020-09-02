#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "ScalarField.hpp"
#include "VectorField.hpp"

#ifndef FLUIDGRID_HPP
#define FLUIDGRID_HPP

class FluidGrid {
    private:
        int iterations;
        int gridSize; // Length & width of cells that will have fluid in them
                        // This will not count the boundary cells
        float gravity[2]; // X and Y component of gravity
        float diffusionK; // Diffusion rate of fluid
        VectorField* velField; // Velocity field
        ScalarField* densityField;
        VectorField* sVelField; // Source velocity field
        ScalarField* sDensityField; // Source density field

    public:
        FluidGrid();
        FluidGrid(const float diffusionK, const int gridSize, const float Fx, const float Fy);
        ~FluidGrid();

        void initializeFluidGridWithForce(const float Fx, const float Fy);

        void setForceGravity(const float Fx, const float Fy);
        void setDiffusionRate(const float diffusionK);
        float getDiffusionRate();
        void setIterations(const int iterations);
        int getIterations();

        void addDensityAndForce(const int x, const int y, const float density, const float Fx, const float Fy);
        //void addDensity(const int x, const int y, const float density);
        //void addForce(const int x, const int y, const float Fx, const float Fy);

        void updateDensityField(const float dt);
        void updateVelocityField(const float dt);

        void diffuse();
        void advect();
        void project();

        void setBoundaries();

        void renderFluid(sf::RenderWindow &window, const sf::Color color);
};

#endif // FLUIDGRID_HPP

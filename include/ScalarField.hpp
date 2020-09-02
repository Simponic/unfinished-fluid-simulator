#include "VectorField.hpp"
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#ifndef SCALARFIELD_HPP
#define SCALARFIELD_HPP

using namespace std;

class ScalarField {
    private:
        int width;
        int height;
        vector< vector<float> > fieldValues;
    public:
        ScalarField();
        ScalarField(const int width, const int height);
        ScalarField(ScalarField &copy);
        ~ScalarField();

        void initializeScalarField();

        int getWidth();
        int getHeight();

        vector< vector<float> > getFieldValues();

        void setValueAtIndex(const int x, const int y, const float value);
        float getValueAtIndex(const int x, const int y);

        void setBoundaries();
        ScalarField* diffuseScalarField(const float diffusionK, const float dt, const int iterations);
        ScalarField* advectScalarField(const float dt, VectorField *vel);
        void addScalarSource(ScalarField *b, const float dt);

        void renderScalarField(sf::RenderWindow &window, const float tileWidth, const sf::Color color);

};

#endif // SCALARFIELD_HPP

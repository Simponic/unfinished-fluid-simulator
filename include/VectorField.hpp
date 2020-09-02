#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#ifndef VECTORFIELD_HPP
#define VECTORFIELD_HPP

#define PI 3.141592654

using namespace std;

class VectorField {
	private:
		int width;
		int height;
		vector< vector<float> > VectorXValues;
		vector< vector<float> > VectorYValues;
	public:
		VectorField();
		VectorField(const int width, const int height);
		VectorField(VectorField &copy);
		~VectorField();

		void initializeVectors();

		void setXComponentAtIndex(const int x, const int y, const float value);
		float getXComponentAtIndex(const int x, const int y);
		void setYComponentAtIndex(const int x, const int y, const float value);
		float getYComponentAtIndex(const int x, const int y);

		vector< vector<float> > getXComponents();
		vector< vector<float> > getYComponents();

		int getWidth();
		int getHeight();

		void setXBoundaries();
		void setYBoundaries();

		void addVectorSource(VectorField *b, const float dt);
		VectorField* diffuse(const float diffusionK, const float dt, const int iterations);

		void renderVectorField(sf::RenderWindow &window, const float tileWidth);

};

#endif // VECTORFIELD_HPP

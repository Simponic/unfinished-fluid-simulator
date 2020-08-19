#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#ifndef BOX_HPP
#define BOX_HPP

class Box {
	private:
		int N; // Length / width of box. Total size will be (N+2) * (N+2)
		int size;
		float* x; // Velocities of each cell
		float* densities;

	public:
		// Setters
		void setN(const int N);
		void setSize(const int size);
		void setVelocityAtIndex(const int x, const int y, const float value);
		void setDensity(const int x, const int y, const float value);

		// Getters
		int getN() const;
		int getSize() const;
		float getVelocityAtIndex(const int x, const int y);
		float getDensity(const int x, const int y);

		void setBounds();
		void diffuse_bad(const float dt, const float diff);
		void addDensities(const float dt);
		std::vector<sf::RectangleShape> renderRectangles (const int width, const int height);

		Box();
		~Box();
};

#endif // BOX_HPP

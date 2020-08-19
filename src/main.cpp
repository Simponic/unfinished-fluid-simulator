#include <SFML/Graphics.hpp>
#include "../include/Box.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {
	sf::RenderWindow window(sf::VideoMode(600, 600), "Fluid Simulator");

	sf::RectangleShape rectangle(sf::Vector2f(0, 0));

	vector<sf::RectangleShape> rectsDraw;

	Box *box = new Box;
	box->addDensities(0.01f);

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			box->setVelocityAtIndex(i, j, 10*sin(i)*cos(j) * (sin(i) * cos(j) < 0 ? -2 : 1));
		}
	}

	while (window.isOpen()) {
		sf::Event event;
	
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		box->diffuse_bad(0.1f, 10);

		rectsDraw = box->renderRectangles(60, 60);

		window.clear(sf::Color::Black);
		for (long unsigned int i = 0; i < rectsDraw.size(); i++) {
			window.draw(rectsDraw.at(i));
		}
		window.display();

	}

	free(box);
	return 0;
}

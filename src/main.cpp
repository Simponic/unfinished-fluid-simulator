#include <SFML/Graphics.hpp>
#include "../include/Box.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {
	sf::RenderWindow window(sf::VideoMode(600, 600), "Fluid Simulator");

	sf::RectangleShape rectangle(sf::Vector2f(0, 0));

	Box box;
	box.addDensities(0.01f);

	while (window.isOpen()) {
		sf::Event event;
	
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);
		rectangle.setFillColor(sf::Color::Blue);
		rectangle.setPosition(sf::Vector2f(100, 100));
		rectangle.setSize(sf::Vector2f(100, 100));

		window.draw(rectangle);
		window.display();

	}

	delete &box;

	return 0;
}

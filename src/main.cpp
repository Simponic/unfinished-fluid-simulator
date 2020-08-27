#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#include "../include/VectorField.hpp"
#include "../include/ScalarField.hpp"
#include "../include/FluidGrid.hpp"

using namespace std;

int main(void) {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Eulerian Simulation");
    window.setVerticalSyncEnabled(true);

    FluidGrid grid(1, 40, 0, 0);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        grid.addDensityAndForce(10, 10, 0.1, 0, 0);
        grid.renderFluid(window, sf::Color(0, 0, 0));

        window.display();
    }
    return 0;
}

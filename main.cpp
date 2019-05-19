
/*
	This is the starting code for the computer.
   	We need to define memory, registers, and other things.
	We will also create the hard disk as a binary file if it doesn't exist.
*/

#include <SFML/Graphics.hpp>

#include "source/Computer.h"

int main(int argc, char** argv) {
	sf::RenderWindow window(sf::VideoMode(400, 400), "Computer");

	// Custom Variables
	Computer computer(65536);

	computer.loadFromDrive();

	while (window.isOpen()) {
		// Event Handling
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::KeyPressed) computer.keyboard((int) event.text.unicode);
			if (event.type == sf::Event::KeyReleased) computer.keyboard(0);
		}

		// Logic
		if (computer.step(window) == 1) window.close();

		// Rendering
		window.clear();

		window.display();
	}
	
	return 0;
}


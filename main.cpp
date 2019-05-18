
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
	Computer computer;

	while (window.isOpen()) {
		// Event Handling
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		// Logic

		// Rendering
		window.clear();

		window.display();
	}
	
	return 0;
}


#include <Window.h>

int test() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

	sf::CircleShape circle(10);
	circle.setPosition(400, 300);
	circle.setFillColor(sf::Color::Red);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color::White);
		window.draw(circle);
		window.display();
	}
	return EXIT_SUCCESS;
}

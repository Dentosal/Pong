#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#define PORT 8171

int main() {
	// Define some constants
	sf::Vector2i windowsize(1024, 768);

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(windowsize.x, windowsize.y, 32), "");
	window.setVerticalSyncEnabled(true);

	// Load the text font
	sf::Font font;
	if (!font.loadFromFile("resources/font.ttf")) {
		return EXIT_FAILURE;
	}

	// read ip from user
	// now in /DEBUG/MODE/
	std::string ip="194.197.235.49";

	// create socket and connect
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(ip, PORT);
	if (status != sf::Socket::Done)	{
		std::cout << "Connection creation error" << std::endl; 	// error
		window.close();
		return EXIT_FAILURE;
	}	
	sf::Packet p;
	socket.receive(p);
	int statusmsg;
	p >> statusmsg;
	std::cout << statusmsg << std::endl;

	sf::RectangleShape left;
	left.setPosition(10, 334);
	left.setSize(sf::Vector2f(5, 100));
	left.setOutlineThickness(3);
	left.setOutlineColor(sf::Color::White);
	left.setFillColor(sf::Color::Blue);

	sf::RectangleShape right;
	right.setPosition(10, 334);
	right.setSize(sf::Vector2f(5, 100));
	right.setOutlineThickness(3);
	right.setOutlineColor(sf::Color::White);
	right.setFillColor(sf::Color::Blue);

	const int radius = 10;
	sf::CircleShape ball;
	ball.setRadius(radius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(sf::Color::White);
	ball.setFillColor(sf::Color::Blue);
	ball.setOrigin(radius / 2, radius / 2);


	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed or escape key pressed: exit
			if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
				window.close();
				break;
			}
			if (event.type==sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {

			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		}
		sf::Packet p;
		p << 404;
		socket.send(p);
		socket.receive(p);
		std::cout << "/" << std::endl;
		int statusmsg;
		int type;
		p >> statusmsg;
		while (!p.endOfPacket()) {
			p >> type;
			if (type==1) {
				bool l;
				p >> l;
				float x, y, sx, sy;
				p >> x >> y >> sx >> sy;
				if (l) {
					left.setPosition(sf::Vector2f(x, y));
					left.setSize(sf::Vector2f(sx, sy));
				}
				else {
					right.setPosition(sf::Vector2f(x, y));
					right.setSize(sf::Vector2f(sx, sy));
				}
			}
			else if (type==2) {
				float x, y;
				p >> x >> y;
				ball.setPosition(sf::Vector2f(x, y));
			}
		} 

		// Clear the window
		window.clear(sf::Color::Black);

		window.draw(left);
		window.draw(right);
		window.draw(ball);
		window.display();
	}



	window.close();
	return EXIT_SUCCESS;
}
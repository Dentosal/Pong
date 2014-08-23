#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "ball.h"
#include "paddle.h"

#define PI 3.141592653589793

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	// Define some constants
	sf::Vector2i windowsize(1024, 768);

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(windowsize.x, windowsize.y, 32), "");
	window.setVerticalSyncEnabled(true);

	// Load the sounds used in the game
	sf::SoundBuffer ballSoundBuffer;
	if (!ballSoundBuffer.loadFromFile("resources/30.wav")) {
		return EXIT_FAILURE;
	}

	// Create paddles
	Paddle leftPaddle(true);
	Paddle rightPaddle(false);
	// Create ball
	Ball ball(ballSoundBuffer);
	ball.setPos(sf::Vector2f(windowsize.x/2, windowsize.y/2));

	// lasers
	std::vector<sf::RectangleShape> lasers;


	bool isLeft = true;

	// Load the text font
	sf::Font font;
	if (!font.loadFromFile("resources/font.ttf"))
		return EXIT_FAILURE;

	sf::Text scoreMsg;
	scoreMsg.setFont(font);
	scoreMsg.setCharacterSize(40);
	scoreMsg.setPosition(50, 50);
	scoreMsg.setColor(sf::Color::Green);
	scoreMsg.setString("0 0");


	sf::Clock clock;
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
				sf::RectangleShape laser(sf::Vector2f(10, 2));
				if (isLeft) {
					laser.setPosition(leftPaddle.getPos()+leftPaddle.getSize()/2.0f);
					laser.rotate(0);
				}
				else {
					laser.setPosition(rightPaddle.getPos()+rightPaddle.getSize()/2.0f);
					laser.rotate(180);
				}
				laser.setFillColor(sf::Color::Green);
				float a = laser.getRotation();
				a=a*(PI/180);
				float y=sin(a);
				float x=cos(a);
				for (int i = 0; i < 40; ++i)
				{
					laser.move(sf::Vector2f(x, y));
				}
				lasers.push_back(laser);
			}
		}
		// get timedelta, reset clock
		float deltaTime = clock.restart().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			leftPaddle.up(deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			leftPaddle.down(deltaTime);
		}

		ball.liiku(deltaTime);

		// Clear the window
		window.clear(sf::Color::Black);
		// And draw everything
		window.draw(scoreMsg);
		// Draw and move lasers
		for (int i = 0; i < lasers.size(); ++i) {
			window.draw(lasers.at(i));
			float a = lasers.at(i).getRotation();
			if (a==0) {
				lasers.at(i).move(sf::Vector2f(deltaTime*500, 0));
			}
			else {
				lasers.at(i).move(sf::Vector2f(deltaTime*500, 0));				
			}
		}
		leftPaddle.draw(window);
		rightPaddle.draw(window);
		ball.draw(window);
		window.display();
	}

	return EXIT_SUCCESS;
}

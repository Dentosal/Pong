#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "paddle.h"
#include "ball.h"

#define PI 3.141592653589793
#define PORT 8171

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

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


	// Connect other player
	sf::IpAddress SelfIP = sf::IpAddress::getPublicAddress();	// get own ip
	std::string IP = SelfIP.toString();

	sf::Text ipmsg;
	ipmsg.setFont(font);
	ipmsg.setCharacterSize(40);
	ipmsg.setPosition(50, 50);
	ipmsg.setColor(sf::Color::Green);
	ipmsg.setString("Please connect to\nIP: "+IP);

	window.clear(sf::Color::Black);
	window.draw(ipmsg);
	window.display();

	sf::SocketSelector ss;
	sf::TcpListener listener;
	sf::TcpSocket client;
	if (listener.listen(PORT) != sf::Socket::Done){
		std::cout << "Connection creation error" << std::endl;
		return EXIT_FAILURE;
	}
	std::string dots;
	// while (true) {
	// 	if (ss.wait(sf::milliseconds(1000))) {
			if (listener.accept(client) != sf::Socket::Done)
			{
				std::cout << "Connection creation error" << std::endl;
				return EXIT_FAILURE;
			}
	// 	}
	// 	sf::Event event;
	// 	while (window.pollEvent(event))
	// 	{
	// 		if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
	// 			return EXIT_SUCCESS;
	// 		}
	// 	}

	// 	dots+=".";
	// 	ipmsg.setString("Please connect to\nIP: "+IP+"\n"+dots);
	// 	window.clear(sf::Color::Black);
	// 	window.draw(ipmsg);
	// 	window.display();
	// }
	std::cout << "#" << std::endl;
	ipmsg.setString("Connection ok");
	window.clear(sf::Color::Black);
	window.draw(ipmsg);
	window.display();

	sf::IpAddress cip;
	cip=client.getRemoteAddress();
	std::cout << "Client connected from ip \"" << cip << "\"" << std::endl;

	sf::Packet p;
	p << 0;
	client.send(p);



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


	const bool isLeft = true;

	sf::Text scoreMsg;
	scoreMsg.setFont(font);
	scoreMsg.setCharacterSize(40);
	scoreMsg.setPosition(50, 50);
	scoreMsg.setColor(sf::Color::Green);
	scoreMsg.setString("0 0");

	bool clientbuttons[3] = {false, false, false};

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
					if (!leftPaddle.fainted){
			leftPaddle.up(deltaTime);
					} else {
						leftPaddle.timeFainted += deltaTime;
					}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (!leftPaddle.fainted){
				leftPaddle.down(deltaTime);
			} else {
				leftPaddle.timeFainted += deltaTime;
			}
		}
				
		if (leftPaddle.timeFainted >= 3){
			leftPaddle.timeFainted = 0;
			leftPaddle.fainted = false;
		}
		if (rightPaddle.timeFainted >= 3){
			rightPaddle.timeFainted = 0;
			rightPaddle.fainted = false;
		}

		ball.liiku(deltaTime, leftPaddle, rightPaddle);

		// Clear the window
		window.clear(sf::Color::Black);
		// And draw everything
		window.draw(scoreMsg);
		// Draw and move lasers
		std::vector<int> RMIndex;
				
		for (int i = 0; i < lasers.size(); ++i) {
			window.draw(lasers.at(i));
			float a = lasers.at(i).getRotation();
			if (a==0) {
				lasers.at(i).move(sf::Vector2f(deltaTime*500, 0));
			}
			else {
				lasers.at(i).move(sf::Vector2f(deltaTime*500, 0));				
			}
				if (lasers.at(i).getPosition().x >= 1020-rightPaddle.getSize().x-10.f 
						&& lasers.at(i).getPosition().x <= 1024
						&& rightPaddle.getPos().y-5 <= lasers.at(i).getPosition().y+10.f 
						&& rightPaddle.getPos().y+rightPaddle.getSize().y+5.f >= lasers.at(i).getPosition().y-10.f ){
						rightPaddle.fainted = true;
				}
			if (lasers.at(i).getPosition().x >= 1024-rightPaddle.getSize().x-10.f){
				RMIndex.push_back(i);
			}
		}
		for (int i = RMIndex.size()-1; i > 0; --i) {
			lasers.erase(lasers.begin()+i);
		}
		sf::Packet p;
		client.receive(p);
		bool x;
		for (int i = 0; i < 3; ++i)
		{
			p >> clientbuttons[i];
		}
		p.clear();
		p << 0;
		leftPaddle.dump(p);
		rightPaddle.dump(p);
		ball.dump(p);
		client.send(p);

		leftPaddle.draw(window, deltaTime);
		rightPaddle.draw(window, deltaTime);
		ball.draw(window);
		window.display();
	}
	return EXIT_SUCCESS;
}

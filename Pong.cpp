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
#include <sstream>

#define PI 3.141592653589793
#define PORT 8171

static inline std::string toStr(int x) {
	std::stringstream type;
	type << x;
	return type.str();
}

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



	sf::Text scoreMsg;
	scoreMsg.setFont(font);
	scoreMsg.setCharacterSize(40);
	scoreMsg.setColor(sf::Color::Green);
	scoreMsg.setString("0 - 0");
	scoreMsg.setOrigin(sf::Vector2f(scoreMsg.getLocalBounds().left+scoreMsg.getLocalBounds().width/2.0f, scoreMsg.getLocalBounds().top+scoreMsg.getLocalBounds().height/2.0f));
	scoreMsg.setPosition(512, 50);

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
			if (event.type==sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && leftPaddle.canShoot && !leftPaddle.fainted) {
				sf::RectangleShape laser(sf::Vector2f(10, 2));
				laser.setPosition(leftPaddle.getPos()+leftPaddle.getSize()/2.0f);
				leftPaddle.shoot();
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
		if (clientbuttons[0] && rightPaddle.canShoot && !rightPaddle.fainted) {
			sf::RectangleShape laser(sf::Vector2f(10, 2));
			laser.rotate(180);
			laser.setPosition(rightPaddle.getPos()+rightPaddle.getSize()/2.0f);
			laser.setFillColor(sf::Color::Green);
			rightPaddle.shoot();
			float a = laser.getRotation();
			a=a*(PI/180);
			float y=sin(a);
			float x=cos(a);
			for (int i = 0; i < 40; ++i) {
				laser.move(sf::Vector2f(x, y));
			}
			lasers.push_back(laser);
		}
		if (clientbuttons[1]) {
			if (!rightPaddle.fainted){
				rightPaddle.up(deltaTime);
			}
		}
		if (clientbuttons[2]) {
			if (!rightPaddle.fainted){
				rightPaddle.down(deltaTime);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			if (!leftPaddle.fainted){
				leftPaddle.up(deltaTime);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (!leftPaddle.fainted){
				leftPaddle.down(deltaTime);
			}
		}
				
		ball.liiku(deltaTime, leftPaddle, rightPaddle);
		// update score message
		scoreMsg.setString(toStr(leftPaddle.score)+" - "+toStr(rightPaddle.score));
		scoreMsg.setOrigin(sf::Vector2f(scoreMsg.getLocalBounds().left+scoreMsg.getLocalBounds().width/2.0f, scoreMsg.getLocalBounds().top+scoreMsg.getLocalBounds().height/2.0f));

		// Clear the window
		window.clear(sf::Color::Black);
		// And draw everything
		window.draw(scoreMsg);
		// Draw and move and dump lasers
		std::vector<int> RMIndex;
		for (int i = 0; i < 3; ++i) {
			clientbuttons[i] = false;
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
		p << 4 << toStr(leftPaddle.score)+" - "+toStr(rightPaddle.score);
		leftPaddle.dump(p);
		rightPaddle.dump(p);
		ball.dump(p);
		for (int i = 0; i < lasers.size(); ++i) {
					window.draw(lasers.at(i));
					p << 3 << lasers.at(i).getPosition().x << lasers.at(i).getPosition().y;
					float a = lasers.at(i).getRotation();
					if (a==0) {
							lasers.at(i).move(sf::Vector2f(deltaTime*500, 0));
					}
					else {
							lasers.at(i).move(sf::Vector2f(-deltaTime*500, 0));				
					}
					if (lasers.at(i).getPosition().x >= 1020-rightPaddle.getSize().x-10.f 
							&& lasers.at(i).getPosition().x <= 1024
							&& rightPaddle.getPos().y-5.f <= lasers.at(i).getPosition().y 
							&& rightPaddle.getPos().y+rightPaddle.getSize().y+5.f >= lasers.at(i).getPosition().y ){
						rightPaddle.fainted = true;
					}
					if (lasers.at(i).getPosition().x <= leftPaddle.getPos().x + leftPaddle.getSize().x
							&& lasers.at(i).getPosition().x >= 10.f 
							&& leftPaddle.getPos().y-5.f <= lasers.at(i).getPosition().y
							&& leftPaddle.getPos().y+leftPaddle.getSize().y+5.f >= lasers.at(i).getPosition().y ){
						leftPaddle.fainted = true;
					}
					if (lasers.at(i).getPosition().x >= 1024-rightPaddle.getSize().x-10.f
							|| lasers.at(i).getPosition().x <= 10.f){
						RMIndex.push_back(i);
					}
				}
		
		for (int i = RMIndex.size()-1; i > 0; --i) {
			lasers.erase(lasers.begin()+i);
		}
		client.send(p);

		leftPaddle.draw(window, deltaTime);
		rightPaddle.draw(window, deltaTime);
		ball.draw(window);
		window.display();
	}
	return EXIT_SUCCESS;
}

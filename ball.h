
#include "SFML/include/SFML/System/Vector2.hpp"
#include "SFML/include/SFML/Graphics/Transformable.hpp"
#include <iostream>
using namespace std;

#define PI 3.141592653589793
class Ball
{
	public:
		Ball(sf::SoundBuffer);
		~Ball();
		void setPos(sf::Vector2f);
		void liiku(float);
		void draw(sf::RenderWindow&);
	private:
		int radius;
		sf::SoundBuffer SoundBuffer;
		sf::Sound sound;
		sf::CircleShape shape;
		float angle;
                float speed;

};
Ball::Ball(sf::SoundBuffer SB) {
	radius=10;
	SoundBuffer = SB;
	sound.setBuffer(SoundBuffer);
	// set shape properteries
	shape.setRadius(radius - 3);
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color::White);
	shape.setFillColor(sf::Color::Blue);
	shape.setOrigin(radius / 2, radius / 2);

        speed = 200.f;
	angle = (std::rand() % 360) * 2 * PI / 360;
        while (std::abs(std::cos(angle)) < 0.7f) {
		angle = (std::rand() % 360) * 2 * PI / 360;
	}

}
Ball::~Ball() {}
void Ball::setPos(sf::Vector2f v) {
	shape.setPosition(v);
}
void Ball::liiku(float dT) {
    shape.move(dT*std::cos(angle)*speed,dT*std::sin(angle)*speed);
    if (shape.getPosition().y-10.f <= 0 || shape.getPosition().y+10.f >= 768){
        angle = -angle;
    }
    speed += 0.1f;
}
void Ball::draw(sf::RenderWindow& window) {
	window.draw(shape);
}
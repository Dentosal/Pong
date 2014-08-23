class Paddle
{
	public:
		Paddle(bool left);
		~Paddle();
		void setPos(sf::Vector2f);
		void setPos(float, float);
		sf::Vector2f getPos();
		sf::Vector2f getSize();
		void up(float);
		void down(float);
		void draw(sf::RenderWindow&);
	private:
		sf::RectangleShape shape;
		bool left;
};
Paddle::Paddle(bool l) {
	left=l;
	if (left) {
		shape.setPosition(10, 334);
	}
	else {
		shape.setPosition(1009, 334);
	}
	shape.setSize(sf::Vector2f(5, 100));
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color::White);
	shape.setFillColor(sf::Color::Blue);
}
Paddle::~Paddle() {}
void Paddle::setPos(sf::Vector2f v) {
	shape.setPosition(v);
}
sf::Vector2f Paddle::getPos() {
	return shape.getPosition();
}
sf::Vector2f Paddle::getSize() {
	return shape.getSize();
}
void Paddle::up(float dT) {
	shape.move(0, -dT*200.0f);
}
void Paddle::down(float dT) {
	shape.move(0, dT*200.0f);
}
void Paddle::draw(sf::RenderWindow& window) {
	window.draw(shape);
}
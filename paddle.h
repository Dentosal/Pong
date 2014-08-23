class Paddle
{
	public:
		Paddle(bool left);
		~Paddle();
		void setPos(sf::Vector2f);
		void setPos(float, float);
		sf::Vector2f getSize();
		void up(float);
		void down(float);
		void draw(sf::RenderWindow&);
	private:
		sf::RectangleShape shape; 	
};
Paddle::Paddle(bool left) {
	shape.setSize(sf::Vector2f(5, 100));
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color::White);
	shape.setFillColor(sf::Color::Blue);
}
Paddle::~Paddle() {}
void Paddle::setPos(sf::Vector2f v) {
	shape.setPosition(v);
}
sf::Vector2f Paddle::getSize() {
	return shape.getSize();
}
void Paddle::up(float deltatime) {

}
void Paddle::down(float deltatime) {
	
}
void Paddle::draw(sf::RenderWindow& window) {
	window.draw(shape);
}
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
		void draw(sf::RenderWindow&, float);
		bool canShoot();                
                bool fainted;
                float timeFainted;
		float[] getData();
	private:
		sf::RectangleShape shape;
		bool left;
		float laserload;
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
        fainted = false;
        float timeFainted = 0;
	laserload=0;
}
Paddle::~Paddle() {}
void Paddle::setPos(sf::Vector2f v) {
	shape.setPosition(v);
}
bool Paddle::canShoot() {
	if (laserload<0.1) {
		laserload=1.0;
		return true;
	}
	return false;
}
sf::Vector2f Paddle::getPos() {
	return shape.getPosition();
}
sf::Vector2f Paddle::getSize() {
	return shape.getSize();
}
void Paddle::up(float dT) {
	shape.move(0, -dT*200.0f);
	if (shape.getPosition().y<0) {
		shape.setPosition(shape.getPosition().x, 0);
	}
}
void Paddle::down(float dT) {
	shape.move(0, dT*200.0f);
	if (shape.getPosition().y>768-shape.getSize().y) {
		shape.setPosition(shape.getPosition().x, 768-shape.getSize().y);
	}
}
void Paddle::draw(sf::RenderWindow& window, float dT) {
	window.draw(shape);
	if (laserload<0) {
		laserload=0;
	}
	else {
		laserload-=(dT);
	}
}
float[] Paddle::getData() {
	float[] x={shape.getPosition().x, shape.getPosition().y, shape.getSize().x, shape.getSize().y};
}
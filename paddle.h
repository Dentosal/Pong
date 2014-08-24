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
		bool canShoot;
                bool fainted;
                void dump(sf::Packet&);
	private:
		sf::RectangleShape shape;
		bool left;
		float laserload;
                float timeFainted;
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
        timeFainted = 0;
	canShoot = true;
        laserload=0;
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
	if (laserload<=0) {
		laserload=0;
                canShoot=true;
	}
	else {
		laserload-=(dT);
	}	
        if (timeFainted >= 1.5) {
            timeFainted = 0;
            fainted = false;
	}
	if (fainted){
            timeFainted+=(dT);
	}
}
void Paddle::dump(sf::Packet& p) {
	std::cout << getPos().x << "_" << getPos().y << std::endl;
	p << 1 << left << getPos().x << getPos().y << getSize().x << getSize().y;
}
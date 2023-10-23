#pragma once
#include <SFML/Graphics.hpp>

class Blaster {
private:
	sf::RectangleShape blaster;

public:
	Blaster(sf::Vector2f size) {
		blaster.setSize(size);
		blaster.setFillColor(sf::Color::Red);
	}

	void fire(int speed) {
		blaster.move(speed, 0);
	}

	int getRight() {
		return blaster.getPosition().x + blaster.getSize().x;
	}

	int getLeft() {
		return blaster.getPosition().x;
	}

	int getTop() {
		return blaster.getPosition().y;
	}

	int getBottom() {
		return blaster.getPosition().y + blaster.getSize().y;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(blaster);
	}

	void setPos(sf::Vector2f newPos) {
		blaster.setPosition(newPos);
	}


}; #pragma once

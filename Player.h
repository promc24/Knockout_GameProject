#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Swordsman
{

private:
	const float STARTING_SPEED = 600;
	const float STARTING_HEALTH = 100000;

	Vector2f s_Position;

	Sprite s_Sprite;

	Texture s_Texture;

	Vector2f s_Resolution;

	// to track the  directions the player is currently moving in
	bool s_UpPressed;
	bool s_DownPressed;
	bool s_LeftPressed;
	bool s_RightPressed;

	int s_Health;
	int s_MaxHealth;

	// Time of the last hit
	Time s_LastHit;

	// Speed in pixels per second
	float s_Speed;

	float xVelocity = .2f;
	float yVelocity = .2f;

	int s_right = 0;
	int s_left = 0;

	int s_top = 0;
	int s_bottom = 0;

public:

	Swordsman(string filename);

	//to spawn the player at the firs position
	void spawn(Vector2f resolution, int x, int y);

	// track player hits
	bool hit(Time timeHit);

	//Time elapsed since last hit
	Time getLastHitTime();

	// Postion of player with respect to the screen coordinates
	FloatRect getGlobalBound();

	//Set position of Player
	void setPosition(Vector2f position);

	// Origin of the player
	Vector2f getCenter();

	// Angle the player is facing
	float getRotation();

	//Getter function to copy the sprite
	Sprite getSprite();

	// The next four functions manipulate the boolean movement variables
	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	// Stop the player moving in a specific direction
	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	// Getter function for health
	int getHealth();
	//Setter funcation for health
	void setHealth(int num);

	// Update the player each frame
	void update(float elapsedTime, Vector2f resolution);
	// Update for a player with roating chracteristics
	void update(float elapsedTime, Vector2i mousePosition, Vector2f resolution);
	//function to implement teleport
	void ultimate(Swordsman& other);


	//four functions to get four boundaries of player
	void getRight();

	void getLeft();

	void getTop();

	void getBottom();
	//funcation to reset stats
	void resetPlayerStats();

};

Swordsman::Swordsman(string filename)
{
	s_Speed = STARTING_SPEED;
	s_Health = STARTING_HEALTH;
	s_MaxHealth = STARTING_HEALTH;

	// Associate a texture with the sprite
	// !!Watch this space!!
	s_Texture.loadFromFile(filename);
	s_Sprite.setTexture(s_Texture);

	s_right = s_Sprite.getPosition().x + s_Sprite.getGlobalBounds().width;
	s_left = s_Sprite.getPosition().x;

	s_top = s_Sprite.getPosition().y;
	s_bottom = s_Sprite.getPosition().y + s_Sprite.getGlobalBounds().height;


	// Set the origin of the sprite to the centre, 
	// for smooth rotation
	s_Sprite.setOrigin(25, 25);
}

void Swordsman::spawn(Vector2f resolution, int x, int y)
{
	// Place the player in the middle of the arena
	s_Position.x = resolution.x / 2;
	s_Position.y = resolution.y / 2;

	// Strore the resolution for future use
	s_Resolution.x = resolution.x;
	s_Resolution.y = resolution.y;

	s_Sprite.setPosition(x, y);

}

Time Swordsman::getLastHitTime()
{
	return s_LastHit;
}

bool Swordsman::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - s_LastHit.asMilliseconds() > 200)// 2 tenths of second
	{
		s_LastHit = timeHit;
		return true;
	}
	else
	{
		return false;
	}

}

FloatRect Swordsman::getGlobalBound()
{
	return s_Sprite.getGlobalBounds();
}

void Swordsman::setPosition(Vector2f position)
{
	s_Sprite.setPosition(position);
}

Vector2f Swordsman::getCenter()
{
	return s_Position;
}

float Swordsman::getRotation()
{
	return s_Sprite.getRotation();
}

Sprite Swordsman::getSprite()
{
	return s_Sprite;
}


void Swordsman::moveLeft()
{
	s_LeftPressed = true;
}

void Swordsman::moveRight()
{
	s_RightPressed = true;
}

void Swordsman::moveUp()
{
	s_UpPressed = true;
}

void Swordsman::moveDown()
{
	s_DownPressed = true;
}

void Swordsman::stopLeft()
{
	s_LeftPressed = false;
}

void Swordsman::stopRight()
{
	s_RightPressed = false;
}

void Swordsman::stopUp()
{
	s_UpPressed = false;
}

void Swordsman::stopDown()
{
	s_DownPressed = false;
}

int Swordsman::getHealth()
{
	return s_Health;
}


void Swordsman::setHealth(int num)
{
	s_Health = s_Health - num;
}

void Swordsman::update(float elapsedTime, Vector2f resolution)
{

	if (s_UpPressed)
	{
		s_Position.y -= s_Speed * elapsedTime;
	}

	if (s_DownPressed)
	{
		s_Position.y += s_Speed * elapsedTime;
	}

	if (s_RightPressed)
	{
		s_Position.x += s_Speed * elapsedTime;
	}

	if (s_LeftPressed)
	{
		s_Position.x -= s_Speed * elapsedTime;
	}


	// Make sure the player does not go out of screen coordinates
	if (s_Position.x >= 1920 - 150 || s_Position.x <= 0)
	{
		s_Position.x = resolution.x / 2;
	}

	if (s_Position.y >= 1080 - 240 || s_Position.y <= 0)
	{
		s_Position.y = resolution.y / 2;
	}
	s_Sprite.setPosition(s_Position);
}

void Swordsman::update(float elapsedTime, Vector2i mousePosition, Vector2f resolution)
{
	if (s_UpPressed)
	{
		s_Position.y -= s_Speed * elapsedTime;
	}

	if (s_DownPressed)
	{
		s_Position.y += s_Speed * elapsedTime;
	}

	if (s_RightPressed)
	{
		s_Position.x += s_Speed * elapsedTime;
	}

	if (s_LeftPressed)
	{
		s_Position.x -= s_Speed * elapsedTime;
	}

	// Make sure the player does not go out of screen coordinates
	if (s_Position.x >= 1920 - 150 || s_Position.x <= 0)
	{
		s_Position.x = s_Position.x / 2;
	}


	if (s_Position.y >= 1080 - 240 || s_Position.y <= 0)
	{
		s_Position.y = s_Position.y / 2;
	}
	s_Sprite.setPosition(s_Position);

	/*if (s_left = 0 || s_right > 1920)
	{
		s_Position.x = resolution.x / 2;
	}

	if (s_top = 0 || s_bottom > 1080)
	{
		s_Position.y = resolution.y / 2;
	}*/

	s_Sprite.setPosition(s_Position);

	// Calculate the angle the player is facing
	double angle = (atan2(mousePosition.y - s_Resolution.y / 2, mousePosition.x - s_Resolution.x / 2) * 180) / 3.141;

	s_Sprite.setRotation(angle);
}

void Swordsman::ultimate(Swordsman& other)
{
	Vector2f p1 = getCenter();
	other.s_Position.x = p1.x + 100;
	other.s_Position.y = p1.y;
	other.s_Sprite.setPosition(other.s_Position);
}

void Swordsman::getRight()
{
	s_right = s_Sprite.getPosition().x + s_Sprite.getGlobalBounds().width;
}

void Swordsman::getLeft()
{
	s_left = s_Sprite.getPosition().x;
}

void Swordsman::getTop()
{
	s_top = s_Sprite.getPosition().y;
}

void Swordsman::getBottom()
{
	s_bottom = s_Sprite.getPosition().y + s_Sprite.getGlobalBounds().height;
}
void Swordsman::resetPlayerStats()
{
	s_Speed = STARTING_SPEED;
	s_Health = STARTING_HEALTH;
	s_MaxHealth = STARTING_HEALTH;
}
#pragma once

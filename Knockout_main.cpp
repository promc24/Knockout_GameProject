#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h" //Player class to implement characteristics
#include "Blaster.h" //Blaster class to shoot blasters
#include <vector>

using namespace sf;
using namespace std;

int main()
{
	// Screen will be updated based on which of the states the game is in
	enum class State { PAUSED, GAME_OVER, PLAYING };

	// Starting with the GAME_OVER state
	State state = State::GAME_OVER;

	// Set the screen resolution by 1920x1080 
	Vector2f resolution;
	resolution.x = 1920;
	resolution.y = 1080;

	// Create a video mode object
	VideoMode vm(1920, 1080);

	//Antialiaisng smooths the textures and shapes. Depends on compatability with graphics card
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Create and open a window for the game
	RenderWindow window(vm, "Knockout", Style::Fullscreen, settings);

	// Creates a scrolling SFML View for the main action
	//View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Clock class variable to track game time
	Clock clock;
	// tracks time in which  the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Creates a texture for the background
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// Creates a sprite for the background
	Sprite spriteBackground;

	// Attachs the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Assiging and creating the bat sprite
	Texture textureBat;
	textureBat.loadFromFile("graphics/bats6.png");
	Sprite spriteBat;
	spriteBat.setTexture(textureBat);
	spriteBat.setPosition(0, 800);

	// To track bat movement
	bool batActive = false;

	// Set batspeed
	float batSpeed = 0.0f;

	// Texture for the clouds
	Texture textureCloud;

	// Load texture
	textureCloud.loadFromFile("graphics/cloud2.png");

	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++)
	{
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;

	}


	// Health bar
	RectangleShape healthBar;
	float healthBarStartingWidth = 400;
	float healthBarHeight = 80;
	healthBar.setSize(Vector2f(healthBarStartingWidth, healthBarHeight));
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition((420 / 2) - healthBarStartingWidth / 2, 120);

	float healthRemaining = 6.0f;
	float healthBarWidthPerSecond = healthBarStartingWidth / healthRemaining;

	//second health bar
	RectangleShape healthBar2;
	float healthBarStartingWidth2 = 400;
	float healthBarHeight2 = 80;
	healthBar2.setSize(Vector2f(healthBarStartingWidth2, healthBarHeight2));
	healthBar2.setFillColor(Color::Red);
	healthBar2.setPosition((1700) - healthBarStartingWidth2 / 2, 120);


	float healthRemaining2 = 6.0f;
	float healthBarWidthPerSecond2 = healthBarStartingWidth2 / healthRemaining2;

	// Manipulating text on the screen
	Text pauseText;
	Text healthText;
	Text healthText2;
	Text gameoverText;

	Font font;
	font.loadFromFile("fonts/Montague.ttf");

	// Set the font to our message
	pauseText.setFont(font);
	healthText.setFont(font);
	healthText2.setFont(font);
	gameoverText.setFont(font);


	// Assign the actual message
	pauseText.setString("Press Enter to start!");
	healthText.setString("Health=0");
	healthText2.setString("Health=0");


	// Set size of text
	pauseText.setCharacterSize(80);
	healthText.setCharacterSize(60);
	healthText2.setCharacterSize(60);
	gameoverText.setCharacterSize(80);

	// Set color of text
	pauseText.setFillColor(Color::White);
	healthText.setFillColor(Color::White);
	healthText2.setFillColor(Color::White);

	// Position the text to the center

	FloatRect textRect = pauseText.getLocalBounds();

	pauseText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	pauseText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	healthText.setPosition(20, 20);
	healthText2.setPosition(1500, 20);

	// Backgrounds for the text
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(400, 80));
	rect1.setPosition(0, 30);

	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(400, 80));
	rect2.setPosition(1500, 30);

	// Prepare the player
	/*Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player4.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(1920 / 2, 600);

	/*Texture texturePlayer2;
	texturePlayer2.loadFromFile("graphics/player6.png");
	Sprite spritePlayer2;
	spritePlayer2.setTexture(texturePlayer2);
	spritePlayer2.setPosition(1920 / 4, 600);*/

	//Assiging Player texture was transferred to the player class constructor
	// Create an instance of the Player class
	Swordsman player("graphics/player6.png");
	Swordsman player2("graphics/player4.png");
	player.spawn(resolution, 1920 / 4, 400);
	player2.spawn(resolution, 1920 / 2, 600);

	//Blaster class Objects
	std::vector<Blaster> blasterVec;
	bool isFiring = false;

	//Variables to track attack type of players
	bool base_attack = false;
	bool defense = false;
	bool ultimate = false;
	bool base_attack2 = false;
	bool defense2 = false;
	bool ultimate2 = false;
	int lastDrawn = 0;

	SoundBuffer blasterBuffer;
	blasterBuffer.loadFromFile("sound/blaster.wav");//filename
	Sound blaster;
	blaster.setBuffer(blasterBuffer);

	SoundBuffer swordBuffer;
	swordBuffer.loadFromFile("sound/sword2.wav");
	Sound sword;
	sword.setBuffer(swordBuffer);

	//Circle Shape to implement defence for player 1
	CircleShape circle(200);
	circle.setRadius(200);

	// change the number of sides (points) to 100
	circle.setPointCount(100);

	// make it semi-transparent 
	circle.setFillColor(sf::Color(255, 255, 255, 128));

	// smooth the shape 
	//circle.setSmooth(true);

	// where is the shape
	//Vector2f circle_position;
	//Vector2f circle_position2;

	//Circle Shape to implement defence for player 2
	CircleShape circle2(200);
	circle2.setRadius(200);

	// change the number of sides (points) to 100
	circle2.setPointCount(100);

	// make it semi-transparent 
	circle2.setFillColor(sf::Color(255, 255, 255, 128));

	// The main game loop
	while (window.isOpen())
	{
		/************* Input and Event Handling *************/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::PLAYING;
				}

				if (state == State::PLAYING)
				{
				}

			}
		}

		// Closing the window
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle WASD and movement
		if (state == State::PLAYING)
		{

			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				player.moveUp();
				circle.setPosition(20000, 20000);
			}
			else
			{
				player.stopUp();

			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				player.moveDown();
				circle.setPosition(20000, 20000);
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				player.moveLeft();
				circle.setPosition(20000, 20000);
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				player.moveRight();
				circle.setPosition(20000, 20000);
			}
			else
			{
				player.stopRight();
			}
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				base_attack = true;
				sword.play();
				circle.setPosition(20000, 20000);
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				ultimate = true;
				isFiring = true;
				blaster.play();
				circle.setPosition(20000, 20000);
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				defense = true;
				circle.setPosition(player.getCenter());
			}
			//Automation of player 2 ,ovements and attcaks
			srand((int)time(0));
			int r = (rand() % 5);
			if (r == 1)
			{
				player2.moveUp();
				circle2.setPosition(20000, 20000);
			}
			else
			{
				player2.stopUp();
			}
			if (r == 2)
			{
				player2.moveDown();
				circle2.setPosition(20000, 20000);
			}
			else
			{
				player2.stopDown();
			}
			if (r == 3)
			{
				player2.moveLeft();
				circle2.setPosition(20000, 20000);
			}
			else
			{
				player2.stopLeft();
			}
			if (r == 4)
			{
				player2.moveRight();
				circle2.setPosition(20000, 20000);
			}
			else
			{
				player2.stopRight();
			}
			srand((int)time(0));
			int i = (rand() % 4);
			if (i == 1)
			{
				base_attack2 = true;
				sword.play();
				circle2.setPosition(20000, 20000);
			}
			if (i == 2)
			{
				ultimate2 = true;
				player.ultimate(player2);
				circle2.setPosition(20000, 20000);
			}
			if (i == 3)
			{
				defense2 = true;
				circle2.setPosition(player2.getCenter());
			}
		}

		/***************** UPDATING THE FRAME *****************/
		if (state == State::PLAYING)
		{

			// Update the time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Caluclate dt in seconds
			float dtAsSeconds = dt.asSeconds();
			// Subtract from the amount of time remaining
			healthRemaining -= player.getHealth();
			healthRemaining2 -= player2.getHealth();
			// size up the time bar
			//healthBar.setSize(Vector2f(healthBarWidthPerSecond*healthRemaining, healthBarHeight));
			//healthBar2.setSize(Vector2f(healthBarWidthPerSecond2*healthRemaining2, healthBarHeight2));
			healthBar.setSize(Vector2f(player.getHealth(), healthBarHeight));
			healthBar2.setSize(Vector2f(player2.getHealth(), healthBarHeight2));

			// Automate bat movements
			if (!batActive)
			{

				// How fast is the bat
				srand((int)time(0) * 10);
				batSpeed = (rand() % 200) + 200;

				// How high is the bat
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBat.setPosition(2000, height);
				batActive = true;

			}
			else
			{

				spriteBat.setPosition(spriteBat.getPosition().x - (batSpeed * dt.asSeconds()), spriteBat.getPosition().y);

				// Make sure bat stays in the screen boundaries
				if (spriteBat.getPosition().x < -100)
				{
					batActive = false;
				}
			}
			//AUtomate cloud movements
			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!cloudsActive[i])
				{

					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);

					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;

				}
				else
				{
					clouds[i].setPosition(clouds[i].getPosition().x + (cloudSpeeds[i] * dt.asSeconds()), clouds[i].getPosition().y);

					// Make sure the cloud stays in the screen boundaries
					if (clouds[i].getPosition().x > 1920)
					{
						cloudsActive[i] = false;
					}

				}

			}
			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition()); //mainView//);

			// Update the player
			player.update(dtAsSeconds, resolution);
			player2.update(dtAsSeconds, resolution);
			//player.update(dtAsSeconds, Mouse::getPosition(), resolution);

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player (useful in scrolling views)			
			//mainView.setCenter(player.getCenter());

			//Collision detection between players based on attack types
			if ((player.getGlobalBound().intersects(player2.getGlobalBound())) && base_attack == true && base_attack2 == false && defense2 == false && ultimate2 == false)
			{
				player2.setHealth(20);
			}

			if ((player.getGlobalBound().intersects(player2.getGlobalBound())) && base_attack == true && base_attack2 == true)
			{
				player.setHealth(10);
				player2.setHealth(10);
			}

			if ((player.getGlobalBound().intersects(player2.getGlobalBound())) && base_attack == true && defense2 == true)
			{
				player2.setHealth(5);
			}

			if ((player.getGlobalBound().intersects(player2.getGlobalBound())) && base_attack == true && ultimate2 == true)
			{
				player.setHealth(60);
				player2.setHealth(10);
			}

			if (ultimate == true && base_attack2 == true)
			{
				for (int i = 0; i < blasterVec.size(); i++)
				{
					if (blasterVec[i].getRight() > player2.getCenter().x && blasterVec[i].getTop() > player2.getCenter().y && blasterVec[i].getBottom() < (player2.getCenter().y + 240))
					{
						player2.setHealth(60);
					}
				}
				if ((player2.getGlobalBound().intersects(player.getGlobalBound())) && base_attack2 == true && ultimate == true)
				{
					player.setHealth(10);
				}
			}
			if (ultimate == true && base_attack2 == false && defense2 == false && ultimate2 == false)
			{
				for (int i = 0; i < blasterVec.size(); i++)
				{
					if (blasterVec[i].getRight() > player2.getCenter().x && blasterVec[i].getTop() > player2.getCenter().y && blasterVec[i].getBottom() < (player2.getCenter().y + 240))
					{
						player2.setHealth(60);
					}
				}
			}

			if (ultimate == true && defense2 == true)
			{
				for (int i = 0; i < blasterVec.size(); i++)
				{
					if (blasterVec[i].getRight() > player2.getCenter().x && blasterVec[i].getTop() > player2.getCenter().y && blasterVec[i].getBottom() < (player2.getCenter().y + 240))
					{
						player2.setHealth(55);
					}
				}
			}

			if (ultimate == true && ultimate2 == true)
			{
				for (int i = 0; i < blasterVec.size(); i++)
				{
					if (blasterVec[i].getRight() > player2.getCenter().x && blasterVec[i].getTop() > player2.getCenter().y && blasterVec[i].getBottom() < (player2.getCenter().y + 240))
					{
						player2.setHealth(60);
					}
				}
				if (player2.getGlobalBound().intersects(player.getGlobalBound()))
				{
					player.setHealth(60);
				}
			}

			if (((player2.getGlobalBound().intersects(player.getGlobalBound())) == true) && (base_attack2 == true) && (base_attack == false) && (defense == false) && (ultimate == false))
			{
				player.setHealth(20);
			}

			if ((player2.getGlobalBound().intersects(player.getGlobalBound())) && base_attack2 == true && defense == true)
			{
				player.setHealth(5);
			}

			if ((player2.getGlobalBound().intersects(player.getGlobalBound())) && (ultimate2 == true) && (base_attack == false) && (defense == false) && (ultimate == false))
			{
				player.setHealth(60);
			}

			if ((player2.getGlobalBound().intersects(player.getGlobalBound())) && ultimate2 == true && defense == true)
			{
				player.setHealth(55);
			}
			lastDrawn++;
		}

		//Update score every 100 frames
		if (lastDrawn == 100) {
			// Update the score text
			stringstream ss;
			stringstream ss2;
			ss << "HEALTH=" << player.getHealth();
			healthText.setString(ss.str());
			ss2 << "HEALTH=" << player2.getHealth();
			healthText2.setString(ss2.str());
			lastDrawn = 0;
		}
		if ((player.getHealth()) <= 0 || (player2.getHealth()) <= 0)
		{
			// Pause the game
			state = State::GAME_OVER;
			// Change the message shown to the player
			if ((player.getHealth()) <= 0)

			{
				stringstream ss3;
				ss3 << "HEALTH=" << 0;
				healthText.setString(ss3.str());
				gameoverText.setString("Game Over!!");

				//Reposition the text based on its new size
				FloatRect textRect = gameoverText.getLocalBounds();
				gameoverText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				gameoverText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}
			if ((player2.getHealth()) <= 0)
			{
				stringstream ss4;
				ss4 << "HEALTH=" << 0;
				healthText2.setString(ss4.str());
				gameoverText.setString("You win!!");

				//Reposition the text based on its new size
				FloatRect textRect = gameoverText.getLocalBounds();
				gameoverText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				gameoverText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

		}
		/*************** Draw the scene based on the state of the game ***************/

		if (state == State::PLAYING)
		{
			window.clear();
			if (isFiring == true) {
				Blaster newBlaster(sf::Vector2f(25, 5));
				newBlaster.setPos(sf::Vector2f(player.getCenter().x + 170, player.getCenter().y + 60));
				blasterVec.push_back(newBlaster);
				isFiring = false;
			}

			// set the mainView to be displayed in the window
			//window.setView(mainView);
			// Draw our game backgpround
			window.draw(spriteBackground);

			// Draw the clouds
			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				window.draw(clouds[i]);
			}

			// Draw backgrounds for the text
			window.draw(rect1);
			window.draw(rect2);

			for (int i = 0; i < blasterVec.size(); i++) {
				blasterVec[i].draw(window);
				blasterVec[i].fire(3);
			}

			// Draw the player
			window.draw(player.getSprite());
			window.draw(player2.getSprite());
			// Drawraw the bat
			window.draw(spriteBat);

			// Draw the health text
			window.draw(healthText);
			window.draw(healthText2);

			window.draw(circle);
			window.draw(circle2);

			// Draw the healthbar
			//window.draw(healthBar);
			//window.draw(healthBar2);

			// Draw our message
			window.draw(gameoverText);

		}

		if (state == State::PAUSED)
		{
			window.draw(spriteBackground);

			// Draw the clouds
			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				window.draw(clouds[i]);
			}

			// Draw backgrounds for the text
			window.draw(rect1);
			window.draw(rect2);

			window.draw(spriteBat);

			// Draw the health
			window.draw(healthText);
			window.draw(healthText2);
			// Draw our message
			window.draw(pauseText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteBackground);

			// Draw the clouds
			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				window.draw(clouds[i]);
			}

			// Draw backgrounds for the text
			window.draw(rect1);
			window.draw(rect2);

			window.draw(spriteBat);

			// Draw the health
			window.draw(healthText);
			window.draw(healthText2);
			// Draw our message
			window.draw(gameoverText);
		}

		window.display();

	}// End  the main game loop

	return 0;
}
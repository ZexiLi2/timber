// Zexi Li Li
// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <sstream>
#include <iostream>
// Make code easier to type with "using namespace"
using namespace sf;
// Function declaration

const int MaxWidth = 1920;
const int MaxHeight = 1080;
const int PosTimeBar = 980;
const int num_branques = 6;
float fps;
// Where is the player/branch? 
// Left or Right
enum class side {LEFT, RIGHT, NONE};
// Cloud data
struct Cloud
{
	bool active;
	sf::Sprite sprite;
	int speed;
};
int main()
{
	Sprite branches[num_branques];
	side branchPositions[num_branques];
	srand(time(NULL));

	// Play a chop sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// Play death sound
	SoundBuffer ripBuffer;
	ripBuffer.loadFromFile("sound/death3.wav");
	Sound rip;
	rip.setBuffer(ripBuffer);
	
	// Play out of time sound
	SoundBuffer time2Buffer;
	time2Buffer.loadFromFile("sound/out_of_time2.wav");
	Sound time2;
	time2.setBuffer(time2Buffer);
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "MINECRAFT 2 !!1! :O");

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background2.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Arbres extra
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree3.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	spriteTree2.setPosition(50, 250);
	spriteTree2.setScale(.75, .75);
	
	Texture textureTree3;
	textureTree3.loadFromFile("graphics/tree3.png");
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree3);
	spriteTree3.setPosition(1200, 400);
	spriteTree3.setScale(1, 1);

	// Prepare the bees
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee2.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 820);
	spriteBee.setScale(0.15, 0.15);

	Texture textureBee2;
	textureBee2.loadFromFile("graphics/bee3.png");
	Sprite spriteBee2;
	spriteBee2.setTexture(textureBee2);
	spriteBee2.setPosition(0, 820);
	spriteBee2.setScale(0.15, 0.15);

	Texture textureBee3;
	textureBee3.loadFromFile("graphics/bee4.png");
	Sprite spriteBee3;
	spriteBee3.setTexture(textureBee3);
	spriteBee3.setPosition(0, 1220);
	spriteBee3.setScale(0.35, 0.35);
	// Is the bee currently moving?
	bool beeActive = false;
	bool beeActive2 = false;
	bool beeActive3 = false;
	// How fast can the bee fly
	float beeSpeed = 0.0f;
	float beeSpeed2 = 0.0f;
	float beeSpeed3 = 0.0f;
	// make 3 cloud sprites from 1 texture
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud4.png");
	// Cloud data array
	const int numClouds = 7;
	const int distance = 150;
	Cloud spriteCloud[numClouds];
	for (int i = 0; i < numClouds; ++i)
	{
		spriteCloud[i].active = false;
		spriteCloud[i].sprite.setTexture(textureCloud);
		spriteCloud[i].sprite.setPosition(0, 0 * distance);
		spriteCloud[i].speed = 0.0f;
		spriteCloud[i].sprite.setScale(.55, .55);
	}
	
	// Variables to control time itself
		Clock clock;

	// FPS counter
	

	// Time bar
		RectangleShape timeBar;
		float timeBarStartWidth = 400;
		float timeBarHeight = 80;
		timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
		timeBar.setFillColor(Color::Red);
		timeBar.setPosition((MaxWidth / 2) - timeBarStartWidth / 2, PosTimeBar);
		Time gameTimeTotal;
		float timeRemaining = 6.0f;
		float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
	
	// Track whether the game is running
		bool paused = true;

	// Draw some text
		int score = 0;

		Text messageText;
		Text scoreText;
		Text fpsText;
	// We need to choose a font
		Font font;
		font.loadFromFile("fonts/minecraft.ttf");

	// Set the font to our message
		messageText.setFont(font);
		scoreText.setFont(font);
		fpsText.setFont(font);
	// Assign the actual message
		messageText.setString("Press Enter to start!");
		scoreText.setString("Score = 0");
		fpsText.setString("FPS 0");

	// Make it really big
		messageText.setCharacterSize(75);
		scoreText.setCharacterSize(100);
		fpsText.setCharacterSize(50);

	// Choose a color
		messageText.setFillColor(Color::White);
		scoreText.setFillColor(Color::White);
		fpsText.setFillColor(Color::White);

	// Position the text
		FloatRect textRect = messageText.getLocalBounds();
		messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
		messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
		RectangleShape rectanglescore (Vector2f(600, 120));
		rectanglescore.setFillColor(Color(25, 25, 25, 150));
		rectanglescore.setPosition(10, 20);
		scoreText.setPosition(rectanglescore.getPosition() + (rectanglescore.getSize() / 2.f));
		scoreText.setPosition(20, 20);

	// FPS counter

		RectangleShape rectanglefps(Vector2f(335, 60));
		rectanglefps.setFillColor(Color(25, 25, 25, 150));
		rectanglefps.setPosition(1560, 20);
		fpsText.setPosition(1570, 20);

	// Prepare 5 branches
		Texture textureBranch;
		textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch sprite
		for (int i = 0; i < num_branques; i++)
		{
			branches[i].setTexture(textureBranch);
			branches[i].setPosition(-2000, -2000);

			branches[i].setOrigin(220, 20);
		}
	// Prepare the player
		Texture texturePlayer;
		texturePlayer.loadFromFile("graphics/player2.png");
		Sprite spritePlayer;
		spritePlayer.setTexture(texturePlayer);
		spritePlayer.setPosition(700, 700);
		spritePlayer.setScale(0.55, 0.55);

	// The player starts on the left
		side playerSide = side::LEFT;
	// Prepare the gravestone
		Texture textureGrave;
		textureGrave.loadFromFile("graphics/rip3.png");
		Sprite spriteGrave;
		spriteGrave.setTexture(textureGrave);
		spriteGrave.setPosition(700, 850);
		spriteGrave.setScale(0.7, 0.7);

	// Prepare the axe
		Texture textureAxe;
		textureAxe.loadFromFile("graphics/axe2.png");
		Sprite spriteAxe;
		spriteAxe.setTexture(textureAxe);
		spriteAxe.setPosition(700, 820);
		spriteAxe.setScale(0.25, 0.25);
		spriteAxe.setRotation(320.f);
		
	// Line the axe up with the tree
		const float AXE_POSITION_LEFT = 730;
		const float AXE_POSITION_RIGHT = 1075;

	// Prepare the flying log
		Texture textureFLog;
		textureFLog.loadFromFile("graphics/log.png");
		Sprite spriteFLog;
		spriteFLog.setTexture(textureFLog);
		spriteFLog.setPosition(810, 820);
	// Some other useful log related variables
		bool logActive = false;
		float logSpeedX = 1000;
		float logSpeedY = -1500;
	// Control the player input
		bool acceptInput = false;
	// Prepare the sound

	// Out of time
	// Move all the branches down one place
		for (int seed = 1; seed <= 5; seed++)
		{
			for (int j = num_branques; j > 0; j--)
			{
				branchPositions[j] = branchPositions[j - 1];
			}
			// Spawn a new branch at position 0
			// LEFT, RIGHT, or NONE
			srand((int)time(0) + seed);
			int r = (rand() % 5);

			switch (r) {
			case 0:
				branchPositions[0] = side::LEFT;
				break;
			case 1:
				branchPositions[0] = side::RIGHT;
				break;
			default:
				branchPositions[0] = side::NONE;
				break;
			}
		}
	//game loop
		//Aquí dins estan totes les tecles
		while (window.isOpen())
		{
			/*
			****************************************
			Handle the players input
			****************************************
			*/
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				//si la tecla esc es apretada, llavors tancarà el programa
				if (Keyboard::isKeyPressed(Keyboard::Escape))
					window.close();
				// Start the game
				if (Keyboard::isKeyPressed(Keyboard::Return))
				{
					paused = false;
					// Reset the time and the score
					score = 0;
					timeRemaining = 6;
					//Make all the branches disappear
					// Start in second position
					for (int i = 1; i < num_branques; i++)
					{
						branchPositions[i] = side::NONE;
					}
				// Make sure the gravestone is hidden
					spriteGrave.setPosition(675, 2000);
				// Move the player into position
					acceptInput = true;
				}
				if (event.type == Event::KeyReleased && !paused)
				{
					//Listen for key presses again
					acceptInput = true;
					// hide axe
					spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
				}
			}

			// Wrap the player controls to
			// Make sure we are accepting input
			if (acceptInput)
			{
				// More code here next...

				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					// Make sure the player is on the right
					playerSide = side::RIGHT;
					score++;
					// Add to the amount of time remaining
					timeRemaining += (2 / score) + .15;
					spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
					spritePlayer.setPosition(1150, 720);
					// update the branches
					for (int j = num_branques - 1; j > 0; j--) {
						branchPositions[j] = branchPositions[j - 1];
					}// Spawn a new branch at position 0
					// LEFT, RIGHT OR NONE
					int r = (rand() % 5);
					switch (r) {
					case 0:
						branchPositions[0] = side::LEFT;
						break;
					case 1:
						branchPositions[0] = side::RIGHT;
						break;
					default:
						branchPositions[0] = side::NONE;
						break;
					}
					// set the log flying
					spriteFLog.setPosition(810, 720);
					logSpeedX = -5000;
					logActive = true;
					chop.play();
					acceptInput = false;
				}

				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					// Make sure the player is on the left
					playerSide = side::LEFT;
					score++;
					// Add to the amount of time remaining
					timeRemaining += (2 / score) + .15;
					spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
					spritePlayer.setPosition(620, 720);
					// update the branches
					for (int j = num_branques - 1; j > 0; j--) {
						branchPositions[j] = branchPositions[j - 1];
					}// Spawn a new branch at position 0
					// LEFT, RIGHT OR NONE
					int l = (rand() % 5);
					switch (l) {
					case 0:
						branchPositions[0] = side::LEFT;
						break;
					case 1:
						branchPositions[0] = side::RIGHT;
						break;
					default:
						branchPositions[0] = side::NONE;
						break;
					}
					// set the log flying
					spriteFLog.setPosition(810, 720);
					logSpeedX = 5000;
					logActive = true;
					chop.play();

					acceptInput = false;
				}
			}
			for (int i = 0; i < num_branques; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}

			}
					// set the log flying to the left

				// Handle the left cursor key

				// Make sure the player is on the left
			//side playerSide = side::LEFT;
					// Add to the amount of time remaining
			// First handle pressing the right cursor key
			
					
			/*
			****************************************
			Update the scene
			****************************************
			*/
			if (!paused)
			{
			// Measure time
			Time dt = clock.restart();
			Time predt = clock.getElapsedTime();
			// Update the fps
			fps = 1.f / (dt.asSeconds() - predt.asSeconds());
			predt = dt;
			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f) {
				// Pause the game
				paused = true;
				// Change the message shown to the player
				messageText.setString("Out of time!!");
				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(MaxWidth / 2.0f, MaxHeight / 2.0f);
				// Play the out of time sound
				time2.play();

			}
			// Setup the bee
			if (!beeActive)
			{
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 250;

				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(1980, height);
				beeActive = true;
			}
			else
				// Move the bee
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);
			}

			// Has the bee reached the right hand edge of the screen?
			if (spriteBee.getPosition().x < -100)
			{
				beeActive = false;
			}

			// Setup the second bee
			if (!beeActive2)
			{
				srand((int)time(0));
				beeSpeed2 = (rand() % 150) + 350;

				srand((int)time(0) * 12);
				float height = (rand() % 400) + 450;
				spriteBee2.setPosition(1980, height);
				beeActive2 = true;
			}
			else
				// Move the bee 2
			{
				spriteBee2.setPosition(
					spriteBee2.getPosition().x -
					(beeSpeed2 * dt.asSeconds()),
					spriteBee2.getPosition().y);
			}

			// Has the bee 2 reached the right hand edge of the screen?
			if (spriteBee2.getPosition().x < -100)
			{
				beeActive2 = false;
			}

			// Setup the third bee
			if (!beeActive3)
			{
				srand((int)time(0));
				beeSpeed3 = (rand() % 250) + 350;

				srand((int)time(0) * 12);
				float height = (rand() % 400) + 400;
				spriteBee3.setPosition(1980, height);
				beeActive3 = true;
			}
			else
				// Move the bee 3
			{
				spriteBee3.setPosition(
					spriteBee3.getPosition().x -
					(beeSpeed3 * dt.asSeconds()),
					spriteBee3.getPosition().y);
			}

			// Has the bee 3 reached the right hand edge of the screen?
			if (spriteBee3.getPosition().x < -100)
			{
				beeActive3 = false;
			}
			// Set it up ready to be a whole new cloud next frame

// Manage the clouds

			for (int i = 0; i < numClouds; ++i)
			{
				if (!spriteCloud[i].active) {
					spriteCloud[i].speed = rand() % 200;
					float height = rand() % (distance * (i + 1));
					spriteCloud[i].sprite.setPosition(-200, height);
					spriteCloud[i].active = true;
				}
				else {
					spriteCloud[i].sprite.setPosition(
					spriteCloud[i].sprite.getPosition().x + (spriteCloud[i].speed * dt.asSeconds()),
					spriteCloud[i].sprite.getPosition().y);
					if (spriteCloud[i].sprite.getPosition().x > MaxWidth) {
						spriteCloud[i].active = false;
					}
				}
			}
			// Update the score text
			std::stringstream ss;
		
			ss << "Score = " << score;
			scoreText.setString(ss.str());
			
			//fps text		
			std::stringstream ss2;

			ss2 << "FPS: " << fps;
			fpsText.setString(ss2.str());
			// update the branch sprites

			// Handle a flying log				
			if (logActive)
			{
				spriteFLog.setPosition(
					spriteFLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),
					spriteFLog.getPosition().y +
					(logSpeedY * dt.asSeconds())
				);
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;
				// Draw gravestone
				spriteGrave.setPosition(525, 760);
				//hide player
				spritePlayer.setPosition(2000, 660);
				// change text message
				messageText.setString("<player> was squashed by falling block");
				//Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				rip.play();
			}

			}// End if(!paused)
		/*
		 ****************************************
		 Draw the scene
		 ****************************************
		 */

		 // Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		// Draw the clouds
		for (int i = 0; i < numClouds; ++i) {
			window.draw(spriteCloud[i].sprite);
		}
		// Draw rectangle for text
		window.draw(rectanglescore);
		window.draw(rectanglefps);

		// Arbre extra
		window.draw(spriteTree2);

		// Draw the branches
		for (int i = 0; i < num_branques; i++) {
			window.draw(branches[i]);
		}
		// Draw the tree
		window.draw(spriteTree);
		
		
		// Draw the player
		window.draw(spritePlayer);
		// Draw the axe
		window.draw(spriteAxe);
		// Draw the flying log
		window.draw(spriteFLog);
		// Draw the gravestone
		window.draw(spriteGrave);
		// Drawraw the bee
		window.draw(spriteBee);
		window.draw(spriteBee2);
		window.draw(spriteBee3);
		// extra tree
		window.draw(spriteTree3);

		// Draw the score
		window.draw(scoreText);
		if (paused)
		{
			window.draw(messageText);
		}
		
		window.draw(fpsText);
		// Draw the timebar
		window.draw(timeBar);
		// Show everything we just drew
		window.display();

	}
	return 0;
}

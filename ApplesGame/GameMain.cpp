#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>


const std::string RESOURCES_PATH = "Resources/";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float INITIAL_SPEED = 100.f; // Pixels per second
const float PLAYER_SIZE = 20.f;
const float ACCELERATION = 20.f; // Pixels per second
const int NUM_APPLES = 20;
const int NUM_STONES = 5;
const float APPLE_SIZE = 20.f;
const float STONE_SIZE = 30.f;
const float MIN_DISTANCE = 80.f;
const float MIN_DISTANCE_APPLE_STONE = 60.f;
bool bIsPaused = false;
float PauseTime = 5.f;
float PauseTimeLeft = 0.f;

int main()
{
	int seed = (int)time(nullptr);
	srand(seed);
	// Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");
	
	//init UI font 
	sf::Font font;
	if (!font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf"))
	{
		return -1;
	}

	// Init score text
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10, 10);

	// Init player state
	float playerX = SCREEN_WIDTH / 2.f;
	float playerY = SCREEN_HEIGHT / 2.f;
	float playerSpeed = INITIAL_SPEED;
	int playerDirection = 0; // 0 - Right, 1 - Up, 2 - Left, 3 - Down

	// Init player shape
	sf::RectangleShape playerShape;
	playerShape.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
	playerShape.setFillColor(sf::Color::Yellow);
	playerShape.setOrigin(PLAYER_SIZE / 2.f, PLAYER_SIZE / 2.f);
	playerShape.setPosition(playerX, playerY);
	
	// init stones
	float stonesX[NUM_STONES];
	float stonesY[NUM_STONES];
	sf::RectangleShape stoneShape[NUM_STONES];

	for (int i = 0; i < NUM_STONES; ++i)
	{
		bool validPosition = false;

		while (!validPosition)
		{

			stonesX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
			stonesY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;

			validPosition = true;

			//distance to another stones
			for (int j = 0; j < i; ++j)
			{
				float dx = stonesX[i] - stonesX[j];
				float dy = stonesY[i] - stonesY[j];
				float distSq = dx * dx + dy * dy;

				if (distSq < MIN_DISTANCE * MIN_DISTANCE)
				{
					validPosition = false;
					break;
				}
			}
		}

		stoneShape[i].setSize(sf::Vector2f(STONE_SIZE, STONE_SIZE));
		stoneShape[i].setFillColor(sf::Color(100, 100, 100));
		stoneShape[i].setOrigin(STONE_SIZE / 2.f, STONE_SIZE / 2.f);
		stoneShape[i].setPosition(stonesX[i], stonesY[i]);
	}


	//init aples
	float applesX[NUM_APPLES];
	float applesY[NUM_APPLES];
	sf::CircleShape appleShape[NUM_APPLES];

	for (int i = 0; i < NUM_APPLES; ++i)
	{
		bool validPosition = false;

		while (!validPosition)
		{
			validPosition = true;

			
			applesX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
			applesY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;

			//Stones distance check
			for (int j = 0; j < NUM_STONES; ++j)
			{
				float dx = applesX[i] - stonesX[j];
				float dy = applesY[i] - stonesY[j];
				float distSq = dx * dx + dy * dy;

				if (distSq < MIN_DISTANCE_APPLE_STONE * MIN_DISTANCE_APPLE_STONE)
				{
					validPosition = false; 
					break;
				}
			}
		}

	
		appleShape[i].setRadius(APPLE_SIZE / 2.f);
		appleShape[i].setFillColor(sf::Color::Red);
		appleShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
		appleShape[i].setPosition(applesX[i], applesY[i]);
	}

	int numEatenApples = 0;
	
	// Init game clocks
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	// Main loop
	while (window.isOpen())
		if (!bIsPaused)
	{
		// Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Read events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Handle input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerDirection = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			playerDirection = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerDirection = 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			playerDirection = 3;
		}

		if (playerDirection == 0)
		{
			playerX += playerSpeed * deltaTime;
		}
		else if (playerDirection == 1)
		{
			playerY -= playerSpeed * deltaTime;
		}
		else if (playerDirection == 2)
		{
			playerX -= playerSpeed * deltaTime;
		}
		else if (playerDirection == 3)
		{
			playerY += playerSpeed * deltaTime;
		}

		// Check screen borders collision
		if (playerX - PLAYER_SIZE / 2.f < 0.f || playerX + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
			playerY - PLAYER_SIZE / 2.f < 0.f || playerY + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
		{
			bIsPaused = true;
			PauseTimeLeft = PauseTime;
		}

		// Check apples collision
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			
				scoreText.setString("Score: " + std::to_string(numEatenApples));
				float sqauredDistance = (playerX - applesX[i]) * (playerX - applesX[i]) +
					(playerY - applesY[i]) * (playerY - applesY[i]);
				float sqareRadiusSum = (APPLE_SIZE + PLAYER_SIZE) * (APPLE_SIZE + PLAYER_SIZE) / 4.f;
				if (sqauredDistance < sqareRadiusSum)
				{
					//apple respawn
					applesX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
					applesY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;

					appleShape[i].setRadius(APPLE_SIZE / 2.f);
					appleShape[i].setFillColor(sf::Color::Red);
					appleShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
					appleShape[i].setPosition(applesX[i], applesY[i]);

					// Increase eaten apples counter
					numEatenApples++;

					// Increase player speed
					playerSpeed += ACCELERATION;
				}
			
		}

		// Check stones collision
		sf::FloatRect playerRect(
				playerX - PLAYER_SIZE / 2.f,
				playerY - PLAYER_SIZE / 2.f,
				PLAYER_SIZE,
				PLAYER_SIZE);

			for (int i = 0; i < NUM_STONES; ++i)
			{
				sf::FloatRect stoneRect(
					stonesX[i] - STONE_SIZE / 2.f,
					stonesY[i] - STONE_SIZE / 2.f,
					STONE_SIZE,
					STONE_SIZE);

				if (playerRect.intersects(stoneRect))
				{
					bIsPaused = true;
					PauseTimeLeft = PauseTime;
					break;
				}
			}


		window.clear();
		playerShape.setPosition(playerX, playerY);
		for (int i = 0; i < NUM_APPLES; ++i)
		{

		window.draw(appleShape[i]);
			
		}
		for (int i = 0; i < NUM_STONES; ++i)
		{

			window.draw(stoneShape[i]);

		}
		
		window.draw(scoreText);
		window.draw(playerShape);
		window.display();

		}

	else
	{
		// Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		PauseTimeLeft -= deltaTime;
		if (PauseTimeLeft <= 0.f)
		{
			// Reset game state
			numEatenApples = 0;

			// Reset player state
			playerX = SCREEN_WIDTH / 2.f;
			playerY = SCREEN_HEIGHT / 2.f;
			playerSpeed = INITIAL_SPEED;
			playerDirection = 0; // 0 - Right, 1 - Up, 2 - Left, 3 - Down
			bIsPaused = false;


			//Respawn apples
			for (int i = 0; i < NUM_APPLES; ++i)
			{
				bool validPosition = false;

				while (!validPosition)
				{
					validPosition = true;


					applesX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
					applesY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;

					//Stones distance check
					for (int j = 0; j < NUM_STONES; ++j)
					{
						float dx = applesX[i] - stonesX[j];
						float dy = applesY[i] - stonesY[j];
						float distSq = dx * dx + dy * dy;

						if (distSq < MIN_DISTANCE_APPLE_STONE * MIN_DISTANCE_APPLE_STONE)
						{
							validPosition = false;
							break;
						}
					}
				}


				appleShape[i].setRadius(APPLE_SIZE / 2.f);
				appleShape[i].setFillColor(sf::Color::Red);
				appleShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
				appleShape[i].setPosition(applesX[i], applesY[i]);
			}


			//Respawn stones
			for (int i = 0; i < NUM_STONES; ++i)
			{
				bool validPosition = false;

				while (!validPosition)
				{

					stonesX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
					stonesY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;

					validPosition = true;

					//distance to another stones
					for (int j = 0; j < i; ++j)
					{
						float dx = stonesX[i] - stonesX[j];
						float dy = stonesY[i] - stonesY[j];
						float distSq = dx * dx + dy * dy;

						if (distSq < MIN_DISTANCE * MIN_DISTANCE)
						{
							validPosition = false;
							break;
						}
					}
				}

				stoneShape[i].setSize(sf::Vector2f(STONE_SIZE, STONE_SIZE));
				stoneShape[i].setFillColor(sf::Color(100, 100, 100));
				stoneShape[i].setOrigin(STONE_SIZE / 2.f, STONE_SIZE / 2.f);
				stoneShape[i].setPosition(stonesX[i], stonesY[i]);
			}

		}
	}

	return 0;
}

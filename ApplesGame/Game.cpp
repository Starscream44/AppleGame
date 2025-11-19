#include "Game.h"
#include <cassert>
#include "constants.h"

namespace ApplesGame
{

	void RestartGame(Game& game)
	{
		InitPlayer(game.player, game);

		// Init apples
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			InitApple(game.apples[i],game);
		}

		// Init rocks
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRock(game.rocks[i], game);
		}

		game.numEatenApples = 0;
		game.isGameFinished = false;
		game.timeSinceGameFinish = 0;
	}

	void InitGame(Game& game)
	{
		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Pacman.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));
		assert(game.font.loadFromFile(RESOURCES_PATH + "\\Fonts/Roboto-Regular.ttf"));
		assert(game.eatBuffer.loadFromFile(RESOURCES_PATH +"\\Sounds/AppleEat.wav"));
		game.eatSound.setBuffer(game.eatBuffer);
		assert(game.hitBuffer.loadFromFile(RESOURCES_PATH +"\\Sounds/Crash.wav"));
		game.hitSound.setBuffer(game.hitBuffer);

		assert(game.backgroundMusic.openFromFile(RESOURCES_PATH + "\\Sounds/Background.wav"));
		game.backgroundMusic.setLoop(true);
		game.backgroundMusic.setVolume(25.f);
		game.backgroundMusic.play();


		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		InitUI(game.uiState, game.font);
		InitMainMenu(game);

		RestartGame(game);
	}

	void UpdateGame(Game& game, float deltaTime)
	{

		switch (game.currentScreen)
		{
		case GameScreen::MAIN_MENU:
			UpdateMainMenu(game);
			break;

		case GameScreen::GAMEPLAY:
			UpdateGameplay(game, deltaTime);;
			break;

		case GameScreen::VICTORY:
			UpdateVictoryScreen(game);
			break;

		case GameScreen::GAME_OVER:
			UpdateGameOverScreen(game);
			break;
		}
	}
	void UpdateGameplay(Game & game, float deltaTime)
	{ 
		// Update game state
		if (!game.isGameFinished)
		{
			// Handle input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				game.player.direction = PlayerDirection::Right;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				game.player.direction = PlayerDirection::Up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				game.player.direction = PlayerDirection::Left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				game.player.direction = PlayerDirection::Down;
			}

			// Update player state
			switch (game.player.direction)
			{
			case PlayerDirection::Right:
			{
				game.player.position.x += game.player.speed * deltaTime;
				break;
			}
			case PlayerDirection::Up:
			{
				game.player.position.y -= game.player.speed * deltaTime;
				break;
			}
			case PlayerDirection::Left:
			{
				game.player.position.x -= game.player.speed * deltaTime;
				break;
			}
			case PlayerDirection::Down:
			{
				game.player.position.y += game.player.speed * deltaTime;
				break;
			}
			}

			// Find player collisions with apples
			for (int i = 0; i < NUM_APPLES; ++i)
			{
				if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
					game.apples[i].position, APPLE_SIZE / 2.f))
				{
					game.eatSound.play();
					game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
					++game.numEatenApples;
					game.player.speed += ACCELERATION;
				}
			}

			// Find player collisions with rocks
			for (int i = 0; i < NUM_ROCKS; ++i)
			{
				if (IsRectanglesCollide(game.player.position, { PLAYER_SIZE, PLAYER_SIZE },
					game.rocks[i].position, { ROCK_SIZE, ROCK_SIZE }))
				{
					game.hitSound.play();
					game.isGameFinished = true;
					game.timeSinceGameFinish = 0.f;
				}
			}

			// Check screen borders collision
			if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f || game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
				game.player.position.y - PLAYER_SIZE / 2.f < 0.f || game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
			{
				game.hitSound.play();
				game.isGameFinished = true;
				game.timeSinceGameFinish = 0.f;
			}
		}
		else
		{
			if (game.timeSinceGameFinish <= PAUSE_LENGTH)
			{
				game.timeSinceGameFinish += deltaTime;
				game.background.setFillColor(sf::Color::Red);
			}
			else
			{
				// Reset backgound
				game.background.setFillColor(sf::Color::Black);

				RestartGame(game);
			}
		}
		
		UpdateUI(game.uiState, game, deltaTime);
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{

		switch (game.currentScreen)
		{
		case GameScreen::MAIN_MENU:
			DrawMainMenu(game, window);
			break;
		case GameScreen::GAMEPLAY:
			DrawGameplay(game, window);
			break;
		case GameScreen::VICTORY:
			DrawVictoryScreen(game, window);
			break;
		case GameScreen::GAME_OVER:
			DrawGameOverScreen(game, window);
			break;
		}
	}
	void DrawGameplay(Game & game, sf::RenderWindow & window)
	{ 
		window.draw(game.background);
		DrawPlayer(game.player, window);

		for (int i = 0; i < NUM_APPLES; ++i)
		{
			DrawApple(game.apples[i], window);
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			DrawRock(game.rocks[i], window);
		}
		DrawUI(game.uiState, window);

	}


	void DeinializeGame(Game& game)
	{

	}

	void UpdateMainMenu(Game& game) 
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			game.mainMenuSelected = 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			game.mainMenuSelected = 1;

		// Enter selection
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			if (game.mainMenuSelected == 0)
			{
				game.currentScreen = GameScreen::GAMEPLAY;
				RestartGame(game);
			}
			else if (game.mainMenuSelected == 1)
			{
				exit(0);
			}
		}
	}

	void DrawMainMenu(Game& game, sf::RenderWindow& window)
	{
		if (game.mainMenuSelected == 0)
		{
			game.menuButtonStart.setFillColor(sf::Color(100, 100, 100));
			game.menuButtonExit.setFillColor(sf::Color(50, 50, 50));
		}
		else
		{
			game.menuButtonStart.setFillColor(sf::Color(50, 50, 50));
			game.menuButtonExit.setFillColor(sf::Color(100, 100, 100));
		}

		window.draw(game.background); 
		window.draw(game.menuButtonStart);
		window.draw(game.menuButtonExit);
		window.draw(game.menuTextStart);
		window.draw(game.menuTextExit);	
	}


	void UpdateVictoryScreen(Game& game) {}
	void DrawVictoryScreen(Game& game, sf::RenderWindow& window){}

	void UpdateGameOverScreen(Game& game) {}
	void DrawGameOverScreen(Game& game, sf::RenderWindow& window){}

	void InitMainMenu(Game& game)
	{
		// START button
		game.menuButtonStart.setSize(sf::Vector2f(300.f, 60.f));
		game.menuButtonStart.setFillColor(sf::Color(50, 50, 50));
		game.menuButtonStart.setPosition(250.f, 200.f);

		game.menuTextStart.setFont(game.font);
		game.menuTextStart.setString("START");
		game.menuTextStart.setCharacterSize(32);
		game.menuTextStart.setFillColor(sf::Color::White);
		game.menuTextStart.setPosition(330.f, 210.f);

		// EXIT button
		game.menuButtonExit.setSize(sf::Vector2f(300.f, 60.f));
		game.menuButtonExit.setFillColor(sf::Color(50, 50, 50));
		game.menuButtonExit.setPosition(250.f, 300.f);

		game.menuTextExit.setFont(game.font);
		game.menuTextExit.setString("EXIT");
		game.menuTextExit.setCharacterSize(32);
		game.menuTextExit.setFillColor(sf::Color::White);
		game.menuTextExit.setPosition(350.f, 310.f);
	}
}
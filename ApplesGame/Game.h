#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"
#include "UI.h"

namespace ApplesGame
{

	enum class GameScreen
	{
		MAIN_MENU,
		GAMEPLAY,
		VICTORY,
		GAME_OVER
	};

	struct Game
	{
		Player player;
		Apple apples[NUM_APPLES];
		Rock rocks[NUM_ROCKS];

		// Global game data
		int numEatenApples = 0;
		UIState uiState;
		bool isGameFinished = false;
		float timeSinceGameFinish = 0.f;
		sf::RectangleShape background;

		//Resources
		sf::Font font;
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;
		sf::SoundBuffer eatBuffer;
		sf::Sound eatSound;
		sf::SoundBuffer hitBuffer;
		sf::Sound hitSound;
		sf::Music backgroundMusic;
		sf::Texture startMenuTexture;
		sf::Sprite startMenuSprite;

		sf::Texture gameOverTexture;
		sf::Sprite gameOverSprite;

		sf::Texture winTexture;
		sf::Sprite winSprite;

		GameScreen currentScreen = GameScreen::MAIN_MENU;

		int mainMenuSelected = 0; 

		sf::RectangleShape menuButtonStart;
		sf::RectangleShape menuButtonExit;

		sf::Text menuTextStart;
		sf::Text menuTextExit;
	};

	void RestartGame(Game& game);
	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinializeGame(Game& game);

	void InitMainMenu(Game& game);

	void UpdateMainMenu(Game& game);
	void DrawMainMenu(Game& game, sf::RenderWindow& window);

	void UpdateGameplay(Game& game, float deltaTime);
	void DrawGameplay(Game& game, sf::RenderWindow& window);

	void UpdateVictoryScreen(Game& game);
	void DrawVictoryScreen(Game& game, sf::RenderWindow& window);

	void UpdateGameOverScreen(Game& game);
	void DrawGameOverScreen(Game& game, sf::RenderWindow& window);
}
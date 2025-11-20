#include "Apple.h"
#include <cstdlib>
#include "Game.h"



namespace ApplesGame
{


    void InitApple(Apple& apple, const Game& game)
    {
        bool valid = false;

        while (!valid)
        {
            apple.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
            valid = true;

            for (int i = 0; i < NUM_ROCKS; ++i)
            {
                if (IsCirclesCollide(
                    apple.position, APPLE_SIZE * 0.5f,
                    game.rocks[i].position, ROCK_SIZE * 0.5f))
                {
                    valid = false;
                    break;
                }
            }
        }

        apple.sprite.setTexture(game.appleTexture);
        SetSpriteSize(apple.sprite, APPLE_SIZE, APPLE_SIZE);
        SetSpriteRelativeOrigin(apple.sprite, 0.5f, 0.5f);
    }

	void DrawApple(Apple& apple, sf::RenderWindow& window)
	{
		apple.sprite.setPosition(apple.position.x, apple.position.y);
		window.draw(apple.sprite);
	}
}
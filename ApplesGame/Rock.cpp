#include "Rock.h"
#include "Game.h"

namespace ApplesGame
{



    void InitRock(Rock& rock, const Game& game)
    {
        bool valid = false;

        while (!valid)
        {
            rock.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

            valid = !IsCirclesCollide(
                rock.position, ROCK_SIZE * 0.5f,
                game.player.position, PLAYER_SIZE * 0.5f
            );
        }

        rock.sprite.setTexture(game.rockTexture);
        SetSpriteSize(rock.sprite, ROCK_SIZE, ROCK_SIZE);
        SetSpriteRelativeOrigin(rock.sprite, 0.5f, 0.5f);
    }
	void DrawRock(Rock& rock, sf::RenderWindow& window)
	{
		rock.sprite.setPosition(rock.position.x, rock.position.y);
		window.draw(rock.sprite);
	}
}

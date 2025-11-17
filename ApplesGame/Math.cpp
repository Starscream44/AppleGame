#include "Math.h"
#include <cstdlib>
#include <sfml/Graphics.hpp>

namespace ApplesGame
{


	Position2D GetRandomPositionInScreen(float screenWidth, float screenHeight)
	{
		Position2D result;
		result.x = rand() / (float)RAND_MAX * screenWidth;
		result.y = rand() / (float)RAND_MAX * screenHeight;
		return result;
	}

	bool IsRectanglesCollide(Position2D rect1Position, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size)
	{
		float dx = (float)fabs(rect1Position.x - rect2Position.x);
		float dy = (float)fabs(rect1Position.y - rect2Position.y);
		return (dx <= (rect1Size.x + rect2Size.x) / 2.f &&
			dy <= (rect1Size.y + rect2Size.y) / 2.f);
	}

	bool IsCirclesCollide(Position2D circle1Position, float circle1Radius, Position2D circle2Position, float circle2Radius)
	{
		float squareDistance = (circle1Position.x - circle2Position.x) *
			(circle1Position.x - circle2Position.x) +
			(circle1Position.y - circle2Position.y) * (circle1Position.y - circle2Position.y);
		float squareRadiusSum = (circle1Radius + circle2Radius) * (circle1Radius + circle2Radius);
		return squareDistance <= squareRadiusSum;
	}
	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sf::Vector2f scale = {desiredWidth/spriteRect.width, desiredHeight/spriteRect.height};
		sprite.setScale(scale);
	}
	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sprite.setOrigin(spriteRect.width * originX, spriteRect.height * originY);
	}

	sf::Vector2f OurVectorToSf(const Vector2D& v)
	{
		return sf::Vector2f(v.x, v.y);
	}

	sf::Vector2f GetSpriteScale(const sf::Sprite& sprite, const Vector2D& desiredSize)
	{
		const sf::Vector2u textureSize = sprite.getTexture()->getSize();
		const sf::Vector2f spriteScale = { desiredSize.x / textureSize.x, desiredSize.y / textureSize.y };
		return spriteScale;
	}

	sf::Vector2f GetTextOrigin(const sf::Text& text, const Vector2D& relativePosition)
	{
		sf::FloatRect textSize = text.getLocalBounds();
		return {
			(textSize.left + textSize.width) * relativePosition.x,
			(textSize.top + textSize.height) * relativePosition.y,
		};
	}
}
/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "enemy.hpp"

sf::Texture enemy::txt;

enemy::enemy()
{
	if (txt.getSize().x == 0)
	{
		if (!txt.loadFromFile("assets/enemy.png"))
		{
			std::cerr << "NO ENEMY!" << std::endl;
		}
	}
	sprite.setTexture(txt);
	sprite.setScale(0.2f, 0.2f);
	sprite.setPosition(0, 0);
}

void enemy::draw(sf::RenderWindow& win)
{
	win.draw(sprite);
}

void enemy::update(const sf::Vector2f& collectorPos)
{
	sf::Vector2f pos = sprite.getPosition();
	sf::Vector2f direc = collectorPos - pos;

	float lenght = std::sqrt(direc.x * direc.x + direc.y * direc.y);
	if (lenght != 0)
		direc /= lenght;
	float speed = 2.f; /// ENEMY SPEED!!!
	sprite.move(direc * speed);
}

void enemy::setRandomStartPosition(int windowWidth, int windowHeight)
{
	int side = rand() % 4; // 0 = left, 1 = right, 2 = top, 3 = bottom
	float x, y;

	switch (side)
	{
		case 0: // left
			x = -50;
			y = rand() % windowHeight;
			break;
		case 1: // right
			x = windowWidth + 50;
			y = rand() % windowHeight;
			break;
		case 2: // top
			x = rand() % windowWidth;
			y = -50;
			break;
		case 3: // bottom
			x = rand() % windowWidth;
			y = windowHeight + 50;
			break;
	}

	sprite.setPosition(x, y);
}

void enemy::flee(const sf::Vector2f& point)
{
	sf::Vector2f pos = sprite.getPosition();
	sf::Vector2f direction = pos - point; // Flee direction
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0)
		direction /= length;

	float fleeSpeed = 8.0f; 
	sprite.move(direction * fleeSpeed);
}

sf::FloatRect enemy::getBounds() const
{
	return sprite.getGlobalBounds();
}
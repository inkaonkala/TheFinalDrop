/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "items.hpp"

sf::Texture item::shareText;

item::item()
{
	if (shareText.getSize().x == 0)
	{
		if (!shareText.loadFromFile("assets/droplet.png"))
		{
			std::cerr << "NO DROPS!" << std::endl;
		}
	}
	sprite.setTexture(shareText);
	sprite.setScale(0.1f, 0.1f);
	setRandomPosition();
}

void item::draw(sf::RenderWindow& win)
{
	win.draw(sprite);
}

void item::dissapear(const sf::Vector2f& center)
{
	sprite.setPosition(-100.f, -100.f);
}

void item::setRandomPosition()
{
	int winw = 1000;
	int winh = 1000;

	int margin = 10;

	float x = margin + rand() % (winw - 2 * margin);
	float y = margin + rand() % (winh - 2 * margin);
	sprite.setPosition(x, y);
	

}

sf::FloatRect item::getBounds() const
{
	return sprite.getGlobalBounds();
}





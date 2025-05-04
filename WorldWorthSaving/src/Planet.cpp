/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "Planet.hpp"

Planet::Planet() : growthStage(0)
{
	visualRadius = 100.f;

	if (!texture1.loadFromFile("assets/plane1.png") || !texture2.loadFromFile("assets/plane2.png") || !texture3.loadFromFile("assets/plane3.png"))
	{
		std::cerr << "No planet TXT" << std::endl;
	}
	sprite.setTexture(texture1);
	sprite.setOrigin(texture1.getSize().x / 2, texture1.getSize().y / 2);
	sprite.setPosition(500, 500);
	sprite.setScale(0.5, 0.5);
}

void Planet::update(float dTime)
{
	sprite.rotate(-8.0f * dTime);
}

void Planet::draw(sf::RenderWindow& win)
{
	win.draw(sprite);
}

void Planet::grow()
{
	if (growthStage < 3)
	{
		growthStage++;
		updateTxt();
	}
}

void Planet::updateTxt()
{
	switch (growthStage)
	{
		case 1:
			sprite.setTexture(texture1);
			sprite.setScale(0.5f, 0.5f);
			visualRadius = 100.f;
			break;
		case 2:
			sprite.setTexture(texture2);
			sprite.setScale(0.65f, 0.65f);		
			visualRadius = 130.f;
			break;
		case 3:
			default:
			sprite.setTexture(texture3);
			sprite.setScale(1.f, 1.f);
			visualRadius = 160.f;
			break;
	}
}

sf::Vector2f Planet::getCenter() const
{
	return sprite.getPosition();
}

float Planet::getRadius() const
{
	return visualRadius * sprite.getScale().x + 4.f;
}

float Planet::getOffset() const
{
	switch (growthStage)
	{
		case 1: return 15.f;  // first stage: smaller offset
		case 2: return 25.f;  // second stage: image gets bigger
		case 3: return 60.f;  // final stage: largest offset
		default: return 15.f;
	}
}

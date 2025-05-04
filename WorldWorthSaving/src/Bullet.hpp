/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#pragma once

#include <SFML/Graphics.hpp>

struct Bullet
{
	sf::CircleShape shape;
	sf::Vector2f	velocity;

	Bullet(sf::Vector2f pos, sf::Vector2f dir)
	{
		shape.setRadius(6.f);
		shape.setFillColor(sf::Color::Yellow);
		shape.setOrigin(6.f, 6.f);
		shape.setPosition(pos);
		velocity = dir * 300.f;
	}
	void update(float dt)
	{
		shape.move(velocity * dt);
	}

	bool isOffScreen()
	{
		sf::Vector2f pos = shape.getPosition();
		return (pos.x < 0 || pos.x > 1000 || pos.y < 0 || pos.y > 1000);
	}

	sf::FloatRect getBounds() const {
		return shape.getGlobalBounds();
	}
};

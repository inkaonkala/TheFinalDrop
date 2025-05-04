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

#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>

class enemy
{
	private:
		static sf::Texture txt;
		sf::Sprite	sprite; 

		float speed;

	public:
		enemy();
		void draw(sf::RenderWindow& win);
		void move();
		void flee(const sf::Vector2f& center);
		void update(const sf::Vector2f& collectorPos);
		sf::FloatRect getBounds() const;
		void setRandomStartPosition(int windowWidth, int windowHeight);
		void reset(int windowWidth, int windowHeight)
		{
			setRandomStartPosition(windowWidth, windowHeight);
		};

};
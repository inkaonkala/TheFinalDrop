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

class item
{
	private:
		static sf::Texture shareText;
		sf::Sprite	sprite; 

		bool collected = false;
		sf::Clock respawnClock;

	public:
		item();
		void draw(sf::RenderWindow& win);
		void dissapear(const sf::Vector2f& center);
		void setRandomPosition();
		sf::FloatRect getBounds() const; //  COLLISION!!
		bool isCollected() const { return collected; }
		void markCollected() { collected = true; }
		void resetCollected() { collected = false; }
		sf::Clock& getRespawnClock() { return respawnClock; }
		

};
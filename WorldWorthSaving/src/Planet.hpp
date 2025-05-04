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
#include <SFML/Graphics.hpp>


class Planet 
{
	private:
		int growthStage;

		sf::Texture texture1;
		sf::Texture texture2;
		sf::Texture texture3;
		sf::Sprite sprite;

		const float offset = 15.f;
		float 	visualRadius;

		void updateTxt();

	public:

		Planet();
		void update(float );
		void draw(sf::RenderWindow& win);
		void grow();

		sf::Vector2f getCenter() const;
		float getRadius() const;
		float getOffset() const;
		int getStage() const { return growthStage; }

};
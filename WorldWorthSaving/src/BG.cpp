/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "BG.hpp"

BG::BG()
{
	if (!txt.loadFromFile("assets/bc.png"))
	{
		std::cerr << "No BackGround!" << std::endl;
	}
	sprite.setTexture(txt);
	sprite.setPosition(0, 0);
}

void BG::draw(sf::RenderWindow& win)
{
	win.draw(sprite);
}
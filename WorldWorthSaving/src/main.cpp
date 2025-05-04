/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */


#include <iostream>
#include <string>
#include "gameBase.hpp"

int main()
{
	std::srand(static_cast<unsigned>(std::time(NULL)));

	gameBase game;
	while (game.whilstRunning())
	{
		game.update();
		game.render();
	}
	return 0;
}
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
#include <vector>
#include <ctime>
#include <memory>
#include <sstream>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "Planet.hpp"
#include "Players.hpp"
#include "BG.hpp"
#include "Bullet.hpp"
#include "enemy.hpp"
#include "items.hpp"

enum class GameState
{
	Story,
	StartScreen,
	Playing,
	GameOver,
	Winner
};

class gameBase
{
	private:
		GameState	state;
		Planet		planet;
		Players		playerA;
		Players 	playerB;
		BG			bg;
		
		sf::RenderWindow	*win;
		sf::Event			ev;
		sf::VideoMode		videoMode;

		sf::Font			font;
		sf::Text			menuText;

		bool hasBeenShot = false;

		std::vector<Bullet> bullets;
		std::vector<item> items;
		std::vector<enemy> enemies;
		//MOVEMENT!

		//side to side
		bool plA_left = false;
		bool plA_right = false;
		bool plB_left = false;
		bool plB_right = false;

		//up_n_down
		bool plA_up = false;
		bool plA_down = false;
		bool plB_up = false;
		bool plB_down = false;

		//ENDING
		bool endingTriggered = false;
		bool sosSpawned = false;
		bool showEnding = false;
		sf::Sprite sosSprite;
		sf::Texture sosTexture;


		//bool space = false;
		sf::Clock shootCooldown;
		sf::Clock enemySpawnClock;
		sf::Clock itemRespawnClock;
		float spawnCooldown = 2.0f; // one in every 2 sec 
		//for deltaTime, hande it in update
		sf::Clock deltaClock;
		float dTime;

		int points;
		int storyIndex = 0;

		std::vector<std::string> storyTxt = {
			"The earth has dried out.",
//			"For the last desperate attempt an expedition has send to look for water.",
//			"You have travelled for weeks. ",
//			"And finally ...",
//			"(Here I want to add animation of the planet exploding)"
		};

		sf::Music			storydMusic;
		bool				StartMusicStarted = false;

		sf::Music			MenuMusic;
		bool				MenuMusicStarted = false;

		sf::Music			GameMusic;
		bool				GameMusicStarted = false;

		sf::Texture			StartScreen;
		sf::Sprite			screenSprite;
		
	public:

		gameBase();
		virtual ~gameBase();

		const bool whilstRunning() const;
		void pollEvents();
		void update();
		void render();
		void init_var();
		void init_win();
		void drawGame(sf::RenderWindow& win);
		void AimNShoot();
		void updateMove();
		void updateGameplay();
		void movementInput(const sf::Event& ev);
		void drawBullets(sf::RenderWindow* win);
		void handleBullets(float dTime);
		void setItems();
		void handleEnemySpawnAndMovement();
		void handleEnemyCollision();
		void handleItems();
		void PlayerSwap(float dTime);
		void handleMusic();
		void shootCollector();
} ;
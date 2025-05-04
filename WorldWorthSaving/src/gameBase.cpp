/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "gameBase.hpp"

gameBase::gameBase()
{
	this->init_var();
	this->init_win();
}

gameBase::~gameBase()
{
	delete this->win;
}

const bool gameBase::whilstRunning() const
{
	return this->win->isOpen();
}

void gameBase::init_var()
{
	this->state = GameState::Story;

	if (!this->font.loadFromFile("assets/medodi.otf"))
		std::cerr << "Failed to download the font\n";

	StartScreen.loadFromFile("assets/bc_start.png"); // or whatever your path is
	screenSprite.setTexture(StartScreen);
	screenSprite.setPosition(0, 0);

	this->menuText.setFont(this->font);
	this->menuText.setCharacterSize(28);
	this->menuText.setFillColor(sf::Color::White);

	playerA.loadTexes("assets/plA/");
	playerA.standOnPlanet(-3.14159f / 2.f - 0.2f, planet.getCenter(), planet.getRadius(), planet.getOffset());
	playerA.setRole(Role::Shooter);

	playerB.loadTexes("assets/plB/");
	playerB.standOnPlanet(-3.14159f / 2.f + 0.2f, planet.getCenter(), planet.getRadius(), planet.getOffset());
	playerB.setRole(Role::Collector);

	if (!sosTexture.loadFromFile("assets/sos.png"))
	std::cerr << "Failed to load SOS texture!" << std::endl;

	sosSprite.setTexture(sosTexture);
	sosSprite.setScale(0.3f, 0.3f); // adjust as needed
	sosSprite.setPosition(-200.f, -200.f); // offscreen until triggered


	setItems();
	points = 3;


	if (!storydMusic.openFromFile("assets/storymode.wav"))
		std::cerr << "Error: Could not open the music file\n";
	else
		storydMusic.setLoop(true);

	if (!MenuMusic.openFromFile("assets/startMenu.wav"))
		std::cerr << "Error: Could not open the music file\n";
	else
		storydMusic.setLoop(true);

	
}

void gameBase::init_win()
{
	this->videoMode.width = 1000;
	this->videoMode.height =1000;
	this->win = new sf::RenderWindow(this->videoMode, "PWW", sf::Style::Titlebar | sf::Style::Close);
	this->win->setFramerateLimit(60);
}

void gameBase::movementInput(const sf::Event& ev)
{
	if (ev.key.code == sf::Keyboard::B && state == GameState::Playing)
	{
		if (shootCooldown.getElapsedTime().asSeconds() > 0.2f)
		{
		AimNShoot();
			shootCooldown.restart();
		}
	}

	if (ev.type == sf::Event::KeyPressed)
	{
		//shoot
//		if (ev.key.code == sf::Keyboard::BackSpace) space = true;
		//movement
		if (ev.key.code == sf::Keyboard::A) plA_left = true;
		if (ev.key.code == sf::Keyboard::D) plA_right = true;
		if (ev.key.code == sf::Keyboard::W) plA_up = true;
		if (ev.key.code == sf::Keyboard::S) plA_down = true;


		if (ev.key.code == sf::Keyboard::Left) plB_left = true;
		if (ev.key.code == sf::Keyboard::Right) plB_right = true;
		if (ev.key.code == sf::Keyboard::Up) plB_up = true;
		if (ev.key.code == sf::Keyboard::Down) plB_down = true;

	}
	else if (ev.type == sf::Event::KeyReleased)
	{
//		if (ev.key.code == sf::Keyboard::BackSpace)	space = false;

		if (ev.key.code == sf::Keyboard::A) plA_left = false;
		if (ev.key.code == sf::Keyboard::D) plA_right = false;
		if (ev.key.code == sf::Keyboard::W) plA_up = false;
		if (ev.key.code == sf::Keyboard::S) plA_down = false;


		if (ev.key.code == sf::Keyboard::Left) plB_left = false;
		if (ev.key.code == sf::Keyboard::Right) plB_right = false;
		if (ev.key.code == sf::Keyboard::Up) plB_up = false;
		if (ev.key.code == sf::Keyboard::Down) plB_down = false;	

	}	
}

void gameBase::pollEvents()
{
	while (this->win->pollEvent(this->ev))
	{
		movementInput(ev);

		if (this->ev.type == sf::Event::Closed)
			this->win->close();
		
		if (this->ev.type == sf::Event::KeyPressed)
		{
			switch (this->state)
			{
				case GameState::Story:
					if (ev.key.code == sf::Keyboard::Enter)
					{
						storyIndex++;
						if (storyIndex >= storyTxt.size())
							this->state = GameState::StartScreen;
					}
					break;

				case GameState::StartScreen:
					if (ev.key.code == sf::Keyboard::Enter)
						this->state = GameState::Playing;
					break;

				case GameState::Playing:
					if (ev.key.code == sf::Keyboard::H && hasBeenShot == false)
					{
						shootCollector();
						hasBeenShot = true;
					}					
					break;					
				case GameState::GameOver:
					if (ev.key.code == sf::Keyboard::Enter)
						this->state = GameState::StartScreen;
					break;

				case GameState::Winner:
					if (ev.key.code == sf::Keyboard::Enter)
						this->state = GameState::StartScreen;
					break;

				default:
					break;
			}
		}
	}
}

void gameBase::update()
{
	dTime = deltaClock.restart().asSeconds();

	handleMusic();
	//INPUT
	pollEvents();

	if (state == GameState::Playing)
	{

		// PLANET
		if (points == 8) 
		{
			planet.grow();
			points = 3; // reset to 3 as you said
		}	
		planet.update(dTime);
		if (planet.getStage() == 3 && points >= 8 && !sosSpawned)
		{
			sosSprite.setPosition(500.f, 20.f);
			sosSpawned = true;
		}



		updateMove();	
		PlayerSwap(dTime);

		// ITEM MECHANICS
		handleItems();

		// ENEMY SPAWN
		handleEnemySpawnAndMovement();
		handleEnemyCollision();
		handleBullets(dTime);
		
		if (sosSpawned)
		{
			sf::FloatRect collectorBounds = (playerA.getRole() == Role::Collector)
				? playerA.getBounds() : playerB.getBounds();

			if (sosSprite.getGlobalBounds().intersects(collectorBounds))
			{
				sosSpawned = false;
				state = GameState::Winner;
			}
		}

		if (points <= 0)
			state = GameState::GameOver;

	}
}

void gameBase::drawGame(sf::RenderWindow& win)
{
	bg.draw(win);
	planet.draw(win);
	playerA.draw(win);
	playerB.draw(win);
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		enemies[i].draw(win);
	}
	for (item& it : items)
	{
		if (!it.isCollected())
			it.draw(win);
	}
	if (sosSpawned)
		win.draw(sosSprite);
}

void gameBase::drawBullets(sf::RenderWindow* win)
{
	for (auto& b : bullets)
		win->draw(b.shape);
}

void gameBase::AimNShoot()
{
	//std::cout << "in aim N Shoot" << std::endl;
	// Draw aiming line for the Shooter
	sf::Vector2f shooterPos;
	float angle;

	if (playerA.getRole() == Role::Shooter)
	{
		shooterPos = playerA.getPosition();
		angle = playerA.getorbitAngle();
	}
	else
	{
		shooterPos = playerB.getPosition();
		angle = playerB.getorbitAngle();
	}

	sf::Vector2f dir(std::cos(angle), std::sin(angle));
	bullets.push_back(Bullet(shooterPos, dir));

	for (size_t i = 0; i < bullets.size(); )
	{
		bool bulletUsed = false;

		for (size_t j = 0; j < enemies.size(); ++j)
		{
			if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].getBounds()))
			{
				enemies.erase(enemies.begin() + j);
				bulletUsed = true;
				break;
			}
		}

		if (bulletUsed)
			bullets.erase(bullets.begin() + i);
		else
			++i;
	}

}

void gameBase::render()
{
	std::stringstream ss;
	this->win->clear(sf::Color(20, 20, 30));

	switch (this->state)
	{
		case GameState::Story:
			menuText.setString(storyTxt[storyIndex]);
			menuText.setPosition(100.f, 600.f);
			break;

		case GameState::StartScreen:
			//menuText.setString("Press ENTER to begin your mission.");
			//menuText.setPosition(100.f, 600.f);
			win->draw(screenSprite);
			break;

		case GameState::Playing:
			ss << "Bubbles: " << points;
			menuText.setString(ss.str());
			menuText.setPosition(20.f, 20.f);
			drawGame(*win);
			drawBullets(win);
			break;

		case GameState::GameOver:
			menuText.setString("Game Over. Press ENTER to restart.");
			menuText.setPosition(400.f, 600.f);
			break;

		case GameState::Winner:
			menuText.setString("Missions success! \n YOU and the EARTH have been saved! \n");
			menuText.setPosition(100.f, 600.f);
			break;
	
	}

	this->win->draw(menuText);
	this->win->display();
}

void gameBase::updateMove()
{
	// PLAYER A
	if (playerA.getRole() == Role::Shooter)
	{
			float dir = 0.f;
			if (plA_left) dir = -1.f;
			if (plA_right) dir = 1.f;

			if (dir != 0.f)
				playerA.walk(dir, dTime, planet.getCenter(), planet.getRadius(), planet.getOffset());
			else
				playerA.setState(PlayerState::atStart);

	}
	// PLAYER B
	if (playerB.getRole() == Role::Shooter)
	{
			float dir = 0.f;
			if (plB_left) dir = -1.f;
			if (plB_right) dir = 1.f;

			if (dir != 0.f)
				playerB.walk(dir, dTime, planet.getCenter(), planet.getRadius(), planet.getOffset());
			else
				playerB.setState(PlayerState::atStart);
	}

	// COLLECTOR
	if (playerA.getRole() == Role::Collector)
	{
		sf::Vector2f move(0.f, 0.f);

		if (playerA.getState() == PlayerState::Floating)
		{
			sf::Vector2f move(0.f, 0.f);

			if (plA_up)
			    move.y -= 1.f;
			if (plA_down)  
				move.y += 1.f;
			if (plA_left)
				move.x -= 1.f;
			if (plA_right)
				move.x += 1.f;
		
			if (move != sf::Vector2f(0.f, 0.f))
			{
				move /= std::sqrt(move.x * move.x + move.y * move.y);
				sf::Vector2f pos = playerA.getPosition();

				sf::Vector2f toPlanet = planet.getCenter() - pos;
				toPlanet /= std::sqrt(toPlanet.x * toPlanet.x + toPlanet.y * toPlanet.y); // normalize

				float dot = move.x * toPlanet.x + move.y * toPlanet.y;

				if (dot > -0.2f)
				{
					pos += move * 80.f * dTime;
					playerA.setPosition(pos);
				}			
			}
		}
	}
	if (playerB.getRole() == Role::Collector)
	{
		sf::Vector2f move(0.f, 0.f);

		if (playerB.getState() == PlayerState::Floating)
		{
			sf::Vector2f move(0.f, 0.f);

			if (plB_up)
			    move.y -= 1.f;
			if (plB_down)  
				move.y += 1.f;
			if (plB_left)
				move.x -= 1.f;
			if (plB_right)
				move.x += 1.f;
		
			if (move != sf::Vector2f(0.f, 0.f))
			{
				move /= std::sqrt(move.x * move.x + move.y * move.y);
				sf::Vector2f pos = playerB.getPosition();

				sf::Vector2f toPlanet = planet.getCenter() - pos;
				toPlanet /= std::sqrt(toPlanet.x * toPlanet.x + toPlanet.y * toPlanet.y); // normalize

				float dot = move.x * toPlanet.x + move.y * toPlanet.y;

				if (dot > -0.7f)
				{
					pos += move * 80.f * dTime;
					playerB.setPosition(pos);
				}			
			}
		}
	}
}

void gameBase::shootCollector()
{
	Players* shooter;
	Players* collector;

	if (playerA.getRole() == Role::Shooter)
	{
		shooter = &playerA;
		collector = &playerB;
	}
	else
	{
		shooter = &playerB;
		collector = &playerA;
	}

	//place collector at shooters pos
	sf::Vector2f launchPos = shooter->getPosition();
	collector->setPosition(launchPos);

	//set launch state and animation
	collector->setState(PlayerState::Launching);

	//Launch!
	sf::Vector2f center = planet.getCenter();
	sf::Vector2f dir = launchPos - center;

	float lenght = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	sf::Vector2f unitDir = dir / lenght;

	collector->shootToSpace(unitDir * 400.f); // setVelocity(unitDir * 400.f);


}

void gameBase::setItems()
{
	items.clear();
	for (int i = 0; i < 15; ++i)
	{
		items.emplace_back();  // No copy, constructs in-place
		items.back().setRandomPosition();
	}
}

void gameBase::handleItems()
{
	sf::FloatRect collectorBounds;
	if (playerA.getRole() == Role::Collector)
		collectorBounds = playerA.getBounds();
	else
		collectorBounds = playerB.getBounds();

	for (item& it : items)
	{
		if (!it.isCollected() && it.getBounds().intersects(collectorBounds))
		{
			++points;
			it.markCollected();
			it.getRespawnClock().restart(); // its own timer
		}
		else if (it.isCollected() && it.getRespawnClock().getElapsedTime().asSeconds() > 1.0f)
		{
			it.setRandomPosition();
			it.resetCollected();
		}
	}

}

void gameBase::handleBullets(float dTime)
{
	for (size_t b = 0; b < bullets.size(); )
	{
		bool bulletRemoved = false;

		bullets[b].update(dTime);
		if (bullets[b].isOffScreen())
		{
			bullets.erase(bullets.begin() + b);
			bulletRemoved = true;
		}
		else
		{
			for (size_t e = 0; e < enemies.size(); ++e)
			{
				if (bullets[b].getBounds().intersects(enemies[e].getBounds()))
				{
					enemies.erase(enemies.begin() + e);
					bullets.erase(bullets.begin() + b);
					bulletRemoved = true;
					break;
				}
			}
		}

		if (!bulletRemoved)
			++b;
	}
}

void gameBase::handleEnemyCollision()
{
	sf::FloatRect playerABounds = playerA.getBounds();
	sf::FloatRect playerBBounds = playerB.getBounds();

	for (size_t i = 0; i < enemies.size(); )
	{
		if (enemies[i].getBounds().intersects(playerABounds) ||
			enemies[i].getBounds().intersects(playerBBounds))
		{
			enemies.erase(enemies.begin() + i); // Enemy disappears
		}
		else
		{
			++i;
		}
	}
}

void gameBase::handleEnemySpawnAndMovement()
{
	if (hasBeenShot)
	{
		if (enemySpawnClock.getElapsedTime().asSeconds() > spawnCooldown)
		{
			for (int i = 0; i < 2; ++i) // spawn 3 at once
			{
				enemies.emplace_back();
				enemies.back().setRandomStartPosition(win->getSize().x, win->getSize().y);
			}
			enemySpawnClock.restart();
		}

		sf::Vector2f targetPos = (playerA.getRole() == Role::Collector) ? playerA.getPosition() : playerB.getPosition();

		for (enemy& e : enemies)
			e.update(targetPos);
	}
	else
	{
		for (enemy& e : enemies)
			e.flee(planet.getCenter());

		if (enemySpawnClock.getElapsedTime().asSeconds() > 50.f)
		{
			enemies.clear();
			enemySpawnClock.restart();
		}
	}
}

void gameBase::PlayerSwap(float dTime)
{
	bool aReturned = playerA.update(dTime, planet.getCenter(), planet.getRadius());
	bool bReturned = playerB.update(dTime, planet.getCenter(), planet.getRadius());

	if (aReturned || bReturned)
	{
		hasBeenShot = false;

	// Switch roles
		if (playerA.getRole() == Role::Collector)
		{
			playerA.setRole(Role::Shooter);
			playerB.setRole(Role::Collector);
		}
		else
		{
			playerA.setRole(Role::Collector);
			playerB.setRole(Role::Shooter);
		}

	// Reset states to standing on planet
		playerA.setState(PlayerState::atStart);
		playerB.setState(PlayerState::atStart);

//		playerA.setOrbitPosition(-3.14159f / 2.f - 0.2f, planet.getCenter(), planet.getRadius());
//		playerB.setOrbitPosition(-3.14159f / 2.f + 0.2f, planet.getCenter(), planet.getRadius());
		playerA.standOnPlanet(playerA.getorbitAngle(), planet.getCenter(), planet.getRadius(), planet.getOffset());
		playerB.standOnPlanet(playerB.getorbitAngle(), planet.getCenter(), planet.getRadius(), planet.getOffset());
	}
}

void gameBase::handleMusic()
{
	switch (state)
	{
		case GameState::Story:
			if (storydMusic.getStatus() != sf::Music::Playing)
			{
				storydMusic.play();
				MenuMusic.stop();
				GameMusic.stop();
			}
			break;

		case GameState::StartScreen:
			if (MenuMusic.getStatus() != sf::Music::Playing)
			{
				MenuMusic.play();
				storydMusic.stop();
				GameMusic.stop();
			}
			break;

		case GameState::Playing:
			if (GameMusic.getStatus() != sf::Music::Playing)
			{
				GameMusic.play();
				MenuMusic.stop();
				storydMusic.stop();
			}
			break;

		case GameState::GameOver:
		case GameState::Winner:
			// Optional: you could continue GameMusic or play a separate one here
			break;
	}
}

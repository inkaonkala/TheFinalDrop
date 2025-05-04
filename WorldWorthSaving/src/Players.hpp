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
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

enum class Role
{
	Shooter,
	Collector
};

enum class PlayerState
{
	atStart,
	Walking,
	Floating,
	Launching
};

class Players
{
	private:
		
		std::map<PlayerState, std::vector<sf::Texture>> textures;
		sf::Sprite sprite;

		Role role;

		PlayerState currentState;
		bool facingLeft = 0;

		float animationTimer = 0.f;
		int currentFrame = 0;

		sf::Vector2f position;
		sf::Vector2f velocity;
		bool isReturning = false;

		//MOVING
		float orbitAngle = -3.14159f / 2.f;

		float orbitSpeed = 3.5f;

		//shooting
		float aimAngle = -3.14159f / 2.f;
//		float aimOffset = 0.f;

	public:

		Players();

		void loadTexes(const std::string& basePath);
		bool update(float dTime, const sf::Vector2f& planetCenter, float planetRadius);
		void draw(sf::RenderWindow& win);

		void setRole(Role newRole);
		Role getRole() const;

		void setState(PlayerState state);
		PlayerState getState() const;

		void setOrbitPosition(float angle, const sf::Vector2f& center, float radius);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		// Collector
		void shootToSpace(const sf::Vector2f& v);
		void move();
		bool retractToPlanet(const sf::Vector2f planetPos, float planetRadius);
		bool isInSpace() const;

		//Shooter
//		void updateAim(float delta);
		float getAimAngle() const;
		void shoot();
		void walk(float direc, float dTime, const sf::Vector2f& center, float radius, float planetOffset);
		float getorbitAngle() const;

		void standOnPlanet(float angle, const sf::Vector2f& center, float radius, float planetOffset);
		sf::FloatRect getBounds() const;
		void reset(const sf::Vector2f& planetCenter, float planetRadius, float planetOffset);
};

/*

	if (flareCollecter == true && itReturning == false)
	Role temp = player1.getRole();
	player1.setRole(player2.getRole());
	player2.setRole(temp);
	*/

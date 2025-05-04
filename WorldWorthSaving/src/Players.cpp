/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */


#include "Players.hpp"
#include "Planet.hpp"

Players::Players() 
{
	role = Role::Shooter;
	currentState = PlayerState::atStart;
	sprite.setScale(0.3f, 0.3f);
}

void Players::loadTexes(const std::string& basePath)
{
	auto load = [&](PlayerState state, const std::vector<std::string>& filenames)
	{
		for (const std::string& file : filenames)
		{
			sf::Texture tex;
			if (!tex.loadFromFile(basePath + file))
				std::cerr << "NO IMAGES FOR THE PLAYER: " << file << "\n" << std::endl;
			textures[state].push_back(tex);
		}
	};
	
	load(PlayerState::atStart, { "stand.png"} );
	load(PlayerState::Walking, { "walk1.png", "walk2.png"} );
	load(PlayerState::Floating, { "float1.png", "float2.png"} );
	load(PlayerState::Launching, { "launch.png"} );

	if (!textures[PlayerState::atStart].empty())
		sprite.setTexture(textures[PlayerState::atStart][0]);

//	sprite.setOrigin(textureWidth / 2.f, textureHeight / 2.f);
	sf::Vector2u texSize = sprite.getTexture()->getSize();
	sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);

	sprite.setScale(0.3f, 0.3f);


}
		
bool Players::update(float dTime, const sf::Vector2f& planetCenter, float planetRadius)
{
	animationTimer += dTime;
	if (animationTimer > 0.2f && textures[currentState].size() > 1)
	{
		currentFrame = (currentFrame + 1) % textures[currentState].size();
		sprite.setTexture(textures[currentState][currentFrame]);
		animationTimer = 0.f;
	}
	if (role == Role::Collector)
	{
		if (!isReturning)
		{
			position += velocity * dTime;
			sprite.setPosition(position);

			if (position.x < 0.f || position.x > 1000.f || position.y < 0.f || position.y > 1000.f)
			{
				velocity = sf::Vector2f(0.f, 0.f);     // stop movement
				setState(PlayerState::Floating);      // switch to floating mode
				isReturning = true;                   // begin retracting next frame
			}
		}
		else
		{
			
			bool done = retractToPlanet(planetCenter, planetRadius);
			if (done)
				return true;
		}
	}
	return false;
}
		
void Players::draw(sf::RenderWindow& win)
{
	win.draw(sprite);
}

void Players::setRole(Role newRole){
	role = newRole;
}

Role Players::getRole() const 
{
	return role;
}

void Players::setState(PlayerState state)
{
	if (state == currentState)
		return ;

	currentState = state;
	currentFrame = 0;
	animationTimer = 0.f;

	if (!textures[state].empty())
	{
			sprite.setTexture(textures[state][0]);
			sf::Vector2u texSize = sprite.getTexture()->getSize();
			sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
			sprite.setScale((sprite.getScale().x < 0 ? -0.3f : 0.3f), 0.3f);
	}
	
}

PlayerState Players::getState() const
{
	return currentState;
}

void Players::setPosition(sf::Vector2f pos)
{
	position = pos;
	sprite.setPosition(pos);
}

sf::Vector2f Players::getPosition() const
{
	return position;
}

void Players::shootToSpace(const sf::Vector2f& v)
{
	velocity = v;
	isReturning = false;
}

bool Players::retractToPlanet(const sf::Vector2f planetPos, float planetRadius)
{
	if (currentState != PlayerState::Floating)
		return true;

	sf::Vector2f direction = planetPos - position;
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance < 50.f)
	{
		isReturning = false;
		setState(PlayerState::atStart);
		float angle = std::atan2(direction.y, direction.x);
		const float offset = 15.f; // same as before
		float surfaceRadius = planetRadius - offset;
		position.x = planetPos.x + surfaceRadius * std::cos(angle);
		position.y = planetPos.y + surfaceRadius * std::sin(angle);
		sprite.setPosition(position);
		sprite.setRotation(0.f);
		return true;
	}
	else
	{
		sf::Vector2f norm = direction / distance;
		position += norm * 80.f * 0.016f; // smooth return
		sprite.setPosition(position);
	}
	return false;
}

void Players::setOrbitPosition(float angle, const sf::Vector2f& center, float radius)
{
	position.x = center.x + radius * std::cos(angle);
	position.y = center.y + radius * std::sin(angle);
	sprite.setPosition(position);

	// Optional: rotate sprite so feet point to center
//	float degrees = angle * 180.f / 3.14159f;
//	sprite.setRotation(degrees + 90.f);
}

void Players::standOnPlanet(float angle, const sf::Vector2f& center, float radius, float planetOffset)
{
	float adjustedRadius = radius - planetOffset;

	position.x = center.x + adjustedRadius * std::cos(angle);
	position.y = center.y + adjustedRadius * std::sin(angle);

	sprite.setPosition(position);
	sprite.setRotation(0.f); // upright

	setState(PlayerState::atStart); // optional: reset state
}


void Players::walk(float direc, float dTime, const sf::Vector2f& center, float radius, float planetOffset)
{
	// WALK SPEED
	orbitAngle += direc * orbitSpeed * dTime;

	//wrap angle
	if (orbitAngle > 3.14159f * 2)
		orbitAngle -= 3.14159f * 2;
	if (orbitAngle < 0.f)
		orbitAngle += 3.14159f * 2;

	//repos curve
	float adjustRadius = radius - planetOffset;

	position.x = center.x + adjustRadius * std::cos(orbitAngle);
	position.y = center.y + adjustRadius * std::sin(orbitAngle);

	sprite.setPosition(position);
	sprite.setRotation(0.f);

	sf::Vector2f direction = center - position;
	float angleRad = std::atan2(direction.y, direction.x);
	float angleDeg = angleRad * 180.f / 3.14159f;

	// Set rotation so feet point toward center
	sprite.setRotation(angleDeg - 90.f);

	//flip for left
	if (direc < 0.f)
		sprite.setScale(-0.3f, 0.3f);
	else if (direc > 0.f)
		sprite.setScale(0.3f, 0.3f);


	setState(PlayerState::Walking); 
}


float Players::getAimAngle() const
{
	return aimAngle;
}

float Players::getorbitAngle() const
{
	return orbitAngle;
}

sf::FloatRect Players::getBounds() const
{
	return sprite.getGlobalBounds();
}

void Players::reset(const sf::Vector2f& planetCenter, float planetRadius, float planetOffset)
{
	// Reset position to starting orbit position
	orbitAngle = -3.14159f / 2.f;
	setOrbitPosition(orbitAngle, planetCenter, planetRadius + planetOffset);

	// Reset movement and animation
	velocity = {0.f, 0.f};
	currentFrame = 0;
	animationTimer = 0.f;
	facingLeft = false;

	// Reset state
	currentState = PlayerState::atStart;
	isReturning = false;
}
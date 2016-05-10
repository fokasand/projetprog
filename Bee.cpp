#include "Bee.hpp"
#include "Utility/Utility.hpp" // inclus pour buildSprite
#include <Application.hpp>
//constructeur
Bee::Bee(Vec2d centre,
			double rayon,
			 Hive* hive,
			 double energy, double amplitude)
: Collider(centre,rayon),hive_(hive),
speed_(amplitude*Vec2d::fromRandomAngle()),energy_(energy), texture (getAppTexture(getBeeConfig()["texture"].toString()))
{}

//morte si le niveau d'energie est nul
bool Bee::isDead()
{
	if(energy_==0)
	{
		return true;
	}
	return false;
}

//déplacement : calcule nouvelles positions et vitesse
void Bee::move(sf::Time dt)
{
	
}

void Bee::update(sf::Time dt)
{
	
}

void Bee::drawOn(sf::RenderTarget& target) const 
{
	auto beeSprite = buildSprite(centre, rayon, texture);
	
	if (( speed_.angle() >= M_PI/2) or (speed_.angle() <= -M_PI/2))
	{
			beeSprite.scale(1, -1);
	}
	beeSprite.rotate(speed_.angle()/DEG_TO_RAD);
	
    target.draw(beeSprite);
}

j::Value Bee::getBeeConfig() const 
{
	return getAppConfig()["simulation"]["bees"]["generic"];
}
j::Value Bee::getConfig() const
{
	return getBeeConfig();
}

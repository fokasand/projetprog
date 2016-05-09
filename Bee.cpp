#include "Bee.hpp"

//constructeur
Bee::Bee(Vec2d centre,
			double rayon,
			 Hive* hive,
			 double energy, double amplitude)
: Collider(centre,rayon),hive_(hive),
speed_(amplitude*Vec2d::fromRandomAngle()),energy_(energy)
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

void Bee::drawOn(sf::RenderTarget& targetWindow) const 
{
	
}

j::Value Bee::getConfig() const virtual
{
return getAppConfig()["simulation"]["bees"]["generic"];
}

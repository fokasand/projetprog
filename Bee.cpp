#include "Bee.hpp"
#include "Utility/Utility.hpp" // inclus pour buildSprite
#include <Application.hpp>
#include <Random/Random.hpp>
#include "Env.hpp"
//constructeur
Bee::Bee(Vec2d centre,
			double rayon,
			 Hive* hive,
			 double energy, double amplitude)
: Collider(centre,rayon),hive_(hive),
speed_(amplitude*Vec2d::fromRandomAngle()),energy_(energy), texture (getAppTexture(getConfig()["texture"].toString()))
{ }

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
void Bee::update(sf::Time dt)
{
	
}
void Bee::randomMove(sf::Time dt)
{
	//changement aléatoire de direction
	if(bernoulli(getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble()))
	{
		double alpha_max(getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
		double alpha (uniform(-alpha_max,alpha_max));
		double beta;
		Vec2d possible_pos;
		
		//changer la direction du déplacement
		speed_.rotate(alpha);
		possible_pos= centre + speed_*dt.asSeconds();
		//verifier que l'abaeille peut occuper la possition possible_pos
		if(getAppEnv().world_.isFlyable(possible_pos))
		{
			centre=possible_pos;
		} else
		{
			if (bernoulli(0.5))
			{
				beta=PI/4;
			} else
			{
				beta= -PI/4;
			}
			speed_.rotate(beta);
		}
		clamping();
	}
	
	//baisse de l'énergie
	energy_-=0.1*dt.asSeconds();
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

j::Value const& Bee::getBeeConfig() const 
{
	return getAppConfig()["simulation"]["bees"]["generic"];
}
j::Value const&  Bee::getConfig() const
{
	return getBeeConfig();
}

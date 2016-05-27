#include "Bee.hpp"
#include "Utility/Utility.hpp" // inclus pour buildSprite
#include <Application.hpp>
#include <Random/Random.hpp>
#include "Env.hpp"

//constructeur
Bee::Bee(vector<State> states,
		Vec2d centre,
		double rayon,Hive* hive,
		double energy,
		double amplitude,
		sf::Texture texture)
: Collider(centre,rayon),
CFSM (states),
hive_(hive),
energy_(energy),
speed_(amplitude*Vec2d::fromRandomAngle()),
texture_ (texture = getAppTexture(getBeeConfig()["texture"].toString())),
prob(getBeeConfig()["moving behaviour"]["random"]["rotation probability"].toDouble()),
alpha_max(getBeeConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble()),
moveMode_(MoveMode::Rest),
memory_(nullptr),
avoidanceClock_(sf::Time::Zero),
statestring_("bonjour"),
isInHive_(1)
{}

//morte si le niveau d'energie est nul
bool Bee::isDead() const
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
	if (energy_==0)
	{
		hive_->killBee();
	}
	
	//action liée à l'etat courant
	action(dt);
	// movement et perte d'energie
	move(dt);
	
}

//déplacement aléatoire
void Bee::randomMove(sf::Time dt)
{
	//changement aléatoire de direction
	if(bernoulli(getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble()))
	{
		double alpha (uniform(-alpha_max,alpha_max));
		speed_.rotate(alpha);
	}
	movebee(dt);
	//baisse de l'énergie
	energy_-=0.1*dt.asSeconds();
	if (energy_<= 0)
	{
		energy_=0;
	}
}

Vec2d* Bee::getMemory() const
{
	return memory_;
}

bool Bee:: movebee(sf::Time dt) // on modularise car on en a besoin pour targetmove() et randomMove()
{
	double beta(0); // peut etre qu'il faut l'initaliser à 0
	//changer la direction du déplacement	
	Vec2d possible_pos = centre + speed_*dt.asSeconds();
	
	//verifier que l'abeille peut occuper la possition possible_pos
	if(getAppEnv().world_.isFlyable(possible_pos))
	{
		centre=possible_pos;
		clamping();
		return 1;
	} else
	{
		if(bernoulli(prob))
		{	
			beta=PI/4;
		} else
		{
			beta= -PI/4;
		}
		speed_.rotate(beta);
		clamping();
		return 0;
	}
}

//déplacement ciblé
void Bee::targetMove(sf::Time dt, Vec2d target)
{
	if (avoidanceClock_ < sf::Time::Zero )
	{
		speed_ = directionTo(target).normalised() * speed_.length();// on fait appel à la fonction codée dans Vec2d
	}
	else
	{
		avoidanceClock_ -= dt;
	}
	if (!movebee(dt))
	{
		avoidanceClock_ = sf::seconds(getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble()); // en faire un attribut statique?
	}
}

void Bee::drawOn(sf::RenderTarget& target) const 
{
	auto beeSprite = buildSprite(centre, rayon, texture_);
	
	if (( speed_.angle() >= M_PI/2) or (speed_.angle() <= -M_PI/2))
	{
			beeSprite.scale(1, -1);
	}
	beeSprite.rotate(speed_.angle()/DEG_TO_RAD);
	
    target.draw(beeSprite);
    
    if(isDebugOn())
    {
		//couleur et épaisseur dependent de l'état de mouvement de l'abeille
		sf::Color color = (getMoveMode()==Random) ? sf::Color::Black : sf::Color::Blue; 
		double size (rayon+1);
		double thickness = (getMoveMode()==Random) ? 5 : 3;
		auto shape = buildAnnulus(centre, size, color, thickness);
		target.draw(shape);
		Vec2d affiche (centre.x,centre.y +20);
		auto const text = buildText(statestring_, affiche , getAppFont(), 10, sf::Color::White);
		target.draw(text);
	}   
}

//déplacement non aléatoire
void Bee::move(sf::Time dt)
{ 
	switch(moveMode_)
	{
		case MoveMode::Rest:
		{
			changeEnergy(-restloss_);
			break;
		}
		
		case MoveMode::Random:
		{
			randomMove(dt);
			changeEnergy(-moveloss_);
			break;
		}
		
		case MoveMode::Targeted:
		{
			targetMove(dt,target_);
			changeEnergy(-moveloss_);
			break;
		}
	}
}

//retourne le mode de déplacement 
MoveMode Bee::getMoveMode() const
{
	return moveMode_;
}

void Bee::setTout()
{
	restloss_=getConfig()["energy"]["consumption rates"]["idle"].toDouble();
	moveloss_=getConfig()["energy"]["consumption rates"]["moving"].toDouble();
	cons_rate = getConfig()["energy"]["consumption rates"]["eating"].toDouble();
	enmin_hive = getConfig()["energy"]["to leave hive"].toDouble();
	visibility_ = getConfig()["visibility range"].toDouble();

}

void Bee::eat(sf::Time dt)
{
	//nectar disponible dans la ruche
	double available (hive_->getNectar());
	
	//si il reste du nectar manger
	if(available != 0)
	{
		//si il reste asser de nectar pour prelever le taux
		if(available > dt.asSeconds()*cons_rate)
		{
			hive_->takeNectar(dt.asSeconds()*cons_rate);
			//l'energie de l'abeille augmente lorsqu'elle mange
			changeEnergy(dt.asSeconds()*cons_rate);
		} else {
			//si pas assez de nectar en consommer ce qu'il reste
			hive_->takeNectar(available);
			changeEnergy(available);
		}
	}
}

//rend la position de la fleur visible
Vec2d* Bee::visibleFlower()
{
	Collider vision (centre, rayon + visibility_);
	if (getAppEnv().getCollidingFlower(vision) == nullptr)
	{
		return nullptr;
	} else {	
		Vec2d position ((getAppEnv().getCollidingFlower(vision))->getPosition());
		return new Vec2d (position);
	}
}

//faire perdre de l'energie à l'abeille
void Bee::changeEnergy(double quant)
{
	energy_+=quant;
	if (energy_ <0 )
	{
		energy_=0;
	}
}


bool Bee::isBeeInHive()
{
	return isInHive_;
}
//passer une adresse à la mémoire
void Bee::setMemory(Vec2d* address)
{
	memory_= address;
}

j::Value const& Bee::getBeeConfig() const 
{
	return getAppConfig()["simulation"]["bees"]["generic"];
}
j::Value const&  Bee::getConfig() const
{
	return getBeeConfig();
}

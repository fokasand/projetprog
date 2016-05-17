#include "Bee.hpp"
#include "Utility/Utility.hpp" // inclus pour buildSprite
#include <Application.hpp>
#include <Random/Random.hpp>
#include "Env.hpp"

State const Bee::IN_HIVE = createUid();
vector<State> Bee::etats_ =  vector<State> (IN_HIVE);
sf::Time const Bee::delay_ = sf::seconds(getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble());

//constructeur
Bee::Bee(Vec2d centre,
			double rayon,Hive* hive,
			double energy,
			double amplitude,
			sf::Texture texture)
: Collider(centre,rayon),
CFSM (etats_),
hive_(hive),
energy_(energy),
speed_(amplitude*Vec2d::fromRandomAngle()),
texture_ (texture = getAppTexture(getBeeConfig()["texture"].toString())),
prob(getBeeConfig()["moving behaviour"]["random"]["rotation probability"].toDouble()),
alpha_max(getBeeConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble()),
moveMode_(Rest),
memory_(nullptr),
avoidanceClock_(0),

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
void Bee::update(sf::Time dt)
{
	//action liée à l'etat courant
	action(dt);// ou OnState
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
		avoidanceClock_= delay_; // en faire un attribut statique?
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
	}
    
}

//déplacement non aléatoire
void Bee::move(sf::Time dt)
{
	switch(moveMode_)
	{
		case Rest:
		{
			energy_-=restloss_;
			break;
		}
		
		case Random:
		{
			energy_-=moveloss_;
			randomMove(dt);
			break;
		}
		
		case Targeted:
		{
			targetMove(dt);
			break;
		}	
	}
	
}

//retourne le mode de déplacement 
MoveMode Bee::getMoveMode() const
{
	return moveMode_;
}

void Bee::setloss()
{
	restloss_=getConfig()["energy"]["consumption rates"]["idle"].toDouble();
	moveloss_=getConfig()["energy"]["consumption rates"]["moving"].toDouble();
}


j::Value const& Bee::getBeeConfig() const 
{
	return getAppConfig()["simulation"]["bees"]["generic"];
}
j::Value const&  Bee::getConfig() const
{
	return getBeeConfig();
}

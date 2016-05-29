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

Bee::~Bee()
{
	//delete memory_;
}


bool Bee::isDead() const
{
	//morte si le niveau d'energie est nul
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
	//avance dans le sens du vecteur vitesse
	movebee(dt);
}

Vec2d* Bee::getMemory() const
{
	return memory_;
}

bool Bee:: movebee(sf::Time dt) // on modularise car on en a besoin pour targetmove() et randomMove()
{
	double beta(0);
	
	// calcule de la position envisagée en fonction du vecteur vitesse
	Vec2d possible_pos = centre + speed_*dt.asSeconds();
	
	//verifier que l'abeille peut occuper cette position
	if(getAppEnv().world_.isFlyable(possible_pos))
	{
		//si oui bouger le centre du collider abeille
		centre=possible_pos;
		//on s'assure que il fait toujorus partie de la carte
		clamping();
		return 1;
	} else //si la position envisagée n'est pas libre
	{
		//changement de direction dans la sens opposé
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
		avoidanceClock_ = sf::seconds(delay); 
	}
}

void Bee::drawOn(sf::RenderTarget& target) const 
{
	//if (!isInHive_)
	//{
		auto beeSprite = buildSprite(centre, rayon, texture_);
		
		if (( speed_.angle() >= M_PI/2) or (speed_.angle() <= -M_PI/2))
		{
				beeSprite.scale(1, -1);
		}
		beeSprite.rotate(speed_.angle()/DEG_TO_RAD);
		
		target.draw(beeSprite);
		
		if(isDebugOn())
		{
			//couleur et épaisseur du cercle dependent de l'état de mouvement de l'abeille
			sf::Color color = (getMoveMode()==Random) ? sf::Color::Black : sf::Color::Blue; 
			double size (rayon+1);
			double thickness = (getMoveMode()==Random) ? 5 : 3;
			auto shape = buildAnnulus(centre, size, color, thickness);
			target.draw(shape);
			Vec2d affiche (centre.x,centre.y +20);
			auto const text = buildText(statestring_, affiche , getAppFont(), 10, sf::Color::White);
			target.draw(text);
		}   
	//}
}

//déplacement selon l'etat de mouvement courant 
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

//permet d'initialiser les attributs differents selon le type de Bee en utilisant la virtualité de getConfig()
void Bee::setTout()
{
	restloss_=getConfig()["energy"]["consumption rates"]["idle"].toDouble();
	moveloss_=getConfig()["energy"]["consumption rates"]["moving"].toDouble();
	cons_rate = getConfig()["energy"]["consumption rates"]["eating"].toDouble();
	enmin_hive = getConfig()["energy"]["to leave hive"].toDouble();
	visibility_ = getConfig()["visibility range"].toDouble();
	delay = getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble();
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
			//prelever le nectar de la ruche
			hive_->takeNectar(dt.asSeconds()*cons_rate);
			//l'energie de l'abeille augmente lorsqu'elle mange
			changeEnergy(dt.asSeconds()*cons_rate);
		} else {
			//si pas assez de nectar en consommer ce qu'il reste,
			//la quantité minimale de nectar de la ruche est donc 0
			hive_->takeNectar(available);
			changeEnergy(available);
		}
	}
}

//rend la position de la premiere fleur faisant partie du champs de vision
Vec2d* Bee::visibleFlower()
{
	//création d'un collider simulatant de champs de vision
	Collider vision (centre, rayon + visibility_);
	
	//si aucune fleur nefait partie du champs de vision rendre nullptr
	if (getAppEnv().getCollidingFlower(vision) == nullptr)
	{
		return nullptr;
	} else {
		//sinon rendre la position de la premiere fleur appercue
		Vec2d position ((getAppEnv().getCollidingFlower(vision))->getPosition());
		return new Vec2d (position);
	}
}

//faire perdre ou gagner de l'energie à l'abeille
void Bee::changeEnergy(double quant)
{
	energy_+=quant;
	//l'energie ne peut être négative 
	if (energy_ <0 )
	{
		energy_=0;
	}
}

//indique si l'abeille se trouve dans la ruche ou non 
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

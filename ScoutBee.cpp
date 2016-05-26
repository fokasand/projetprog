#include "ScoutBee.hpp"
#include <Application.hpp>
#include "Env.hpp"

State const ScoutBee::IN_HIVE = createUid();
State const ScoutBee::SEARCH_FLOWER = createUid();
State const ScoutBee::TO_HIVE = createUid();
vector<State> ScoutBee::etats_ = vector<State> {IN_HIVE, SEARCH_FLOWER, TO_HIVE};


//constructeur
ScoutBee::ScoutBee(Vec2d centre, Hive* hive) : 
Bee( etats_, centre, getScoutConfig()["size"].toDouble(), hive, 
	getScoutConfig()["energy"]["initial"].toDouble(), 
	getScoutConfig()["speed"].toDouble(),
	getAppTexture(getScoutConfig()["texture"].toString())),  //peut etre dans dessin
	enmin_flower(getScoutConfig()["energy"]["to seek flowers"].toDouble())
{
	setTout();
}

//éxecution des actions liées à l'état courant
void ScoutBee::onState(State current, sf::Time dt)
{
	
	if(current==IN_HIVE)
	{
		std::cerr << "inhive" << std::endl;
		// donner l'addresse à une worker
		{
			statestring_="in_hive_sharing[n]";
		}
		
		if (energy_< enmin_hive)
		{
			std::cerr << "inhive" << std::endl;
			statestring_= "in_hive_eat";
			eat(dt);
		}
		//passer l'adresse avec setmemory_
		if(memory_==nullptr)
		//arrêter de la dessiner dans draw (utilsier continue)
		//passer à l'état suivant (recherche d'une fleur)
		{
			statestring_=="in_hive_leaving";
			nextState();
		}
	}
	
	if(current == TO_HIVE)
	{		
		if(getAppEnv().getCollidingHive(*this) != nullptr)
		{
			std::cerr << "touched hive" << std::endl;
			nextState();
		}
	}
	
	if(current == SEARCH_FLOWER)
	{
		statestring_="seeking_flower";
		if (visibleFlower()!= nullptr or energy_ < enmin_flower)
			{
				setMemory(visibleFlower());
				nextState();
			}
	}
}
void ScoutBee::onEnterState( State state)
{
	{
		if(state==IN_HIVE)
		{
			std::cerr<<"onEnterSTate hive " << std::endl;
			
			//donner l'addresse a une worker dans le tableau //waiting list
			// switch et popback
			moveMode_=MoveMode::Rest;
		}
		
		if(state==SEARCH_FLOWER)
		{
			std::cerr<< "onEnterState search flower " << std::endl;
			//effacer la mémoire on admet que la position en mémoire à étée communiquée
			memory_=nullptr;
			moveMode_=MoveMode::Random;
		}
		
		if(state==TO_HIVE)
		{
			//prendre la ruche comme cible de déplacment
			target_=hive_->getPosition();
			moveMode_= MoveMode::Targeted;
			statestring_="back_to_hive";
		}
		
	}
	
}

//redéfinition de drawon:
void ScoutBee::drawOn(sf::RenderTarget& target) const 
{
	Bee::drawOn(target);
	if (isDebugOn())
	{
		Vec2d affiche (centre.x, centre.y - 25 );
		auto const text = buildText("Scout: energy "+to_nice_string(energy_), affiche , getAppFont(), 10, sf::Color::Blue); // + statestring_
		target.draw(text);
	}
}

//racourci pour les données de configuration
j::Value const& ScoutBee::getScoutConfig()
{
	return getAppConfig()["simulation"]["bees"]["scout"];
}

j::Value const& ScoutBee::getConfig() const 
{
	return getScoutConfig();
}

#include "ScoutBee.hpp"
#include <Application.hpp>

State const ScoutBee::SEARCH_FLOWER = createUid();

//constructeur
ScoutBee::ScoutBee(Vec2d centre, Hive* hive) : 
Bee( centre, getScoutConfig()["size"].toDouble(), hive, 
	getScoutConfig()["energy"]["initial"].toDouble(), 
	getScoutConfig()["speed"].toDouble(),
	getAppTexture(getScoutConfig()["texture"].toString())),  //peut etre dans dessin
	enmin_flower(getScoutConfig()["energy"]["to seek flowers"].toDouble())
{
	setTout();
	states_.push_back(SEARCH_FLOWER);
}

//éxecution des actions liées à l'état courant
void ScoutBee::onState(State current, sf::Time dt)
{
	
	if(current==IN_HIVE)
	{
		std::cerr << "inhive" << std::endl;
		// donner l'addresse à une worker
		{
			statestring_=="in_hive_sharing[n]";
		}
		while (energy_< enmin_hive)
		{
			std::cerr << "inhive" << std::endl;
			statestring_= "in_hive_eat";
			eat(dt);
		}
		//passer l'adresse avec setmemory_
		if(memory_==nullptr)
		//arrêter de la dessiner mais comment
		//passer à l'état suivant (recherche d'une fleur)
			{
				nextState();
			}
	}
	
	if(current == TO_HIVE)
	{
		statestring_="back_to_hive";
	}
	
	if(current == SEARCH_FLOWER)
	{
		std::cerr << "searchin flower" << std::endl;
		statestring_=="seeking_flower";
	}
}
void ScoutBee::onEnterState( State state)
{
	{
		if(state==IN_HIVE)
		{
			//donner l'addresse a une worker dans le tableau //waiting list
			// switch et popback
			moveMode_=Rest;
		}
		
		if(state==SEARCH_FLOWER)
		{
			statestring_=="in_hive_leaving";
			//effacer la mémoire on admet que la position en mémoire à étée communiquée
			memory_=nullptr;
			moveMode_=Random;
			// trouve une fleur 
			if (visibleFlower()!= nullptr)
			{
		
			}
		}
		
		if(state==TO_HIVE)
		{
			//prendre la ruche comme cible de déplacment
			target_=hive_->getPosition();
			moveMode_=Targeted;
		}
		
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

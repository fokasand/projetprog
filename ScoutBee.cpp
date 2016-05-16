#include "ScoutBee.hpp"
#include <Application.hpp>

State const ScoutBee::SEARCH_FLOWER = createUid();
State const ScoutBee::RETURN_HIVE = createUid();

//constructeur
ScoutBee::ScoutBee(Vec2d centre, Hive* hive) : 
Bee( centre, getScoutConfig()["size"].toDouble(), hive, 
	getScoutConfig()["energy"]["initial"].toDouble(), 
	getScoutConfig()["speed"].toDouble(),
	getAppTexture(getScoutConfig()["texture"].toString()))
{
	setloss();
	states_.push_back(SEARCH_FLOWER);
	states_.push_back(RETURN_HIVE);
}

//éxecution des actions liées à l'état courant
void ScoutBee::onState(State current,sf::Time dt)
{
//TODO: à coder	
}
void ScoutBee::onEnterState(State state)
{
	{
		if(state==IN_HIVE)
		{
			moveMode_=Rest;
		}
		
		if(state==SEARCH_FLOWER)
		{
			//effacer la mémoire on admet que la position en mémoire à étée communiquée
			memory_=nullptr;
			moveMode_=Random;
			break;
		}
		
		if(state==RETURN_HIVE)
		{
			//prendre la ruche comme cible de déplacment
			target_=hive_->getPosition();
			moveMode_=Targeted;
		}
		
	}
	
}
//racourci pour les données de configuration
j::Value const& ScoutBee::getScoutConfig() const
{
	return getAppConfig()["simulation"]["bees"]["scout"];
}

j::Value const& ScoutBee::getConfig() const
{
	return getScoutConfig();
}

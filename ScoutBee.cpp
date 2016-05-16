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

//racourci pour les données de configuration
j::Value const& ScoutBee::getScoutConfig() const
{
	return getAppConfig()["simulation"]["bees"]["scout"];
}

j::Value const& ScoutBee::getConfig() const
{
	return getScoutConfig();
}

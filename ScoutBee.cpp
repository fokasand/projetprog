#include "ScoutBee.hpp"
#include <Application.hpp>

State const WorkerBee::IN_HIVE = createUid()

ScoutBee::ScoutBee(Vec2d centre, Hive* hive,vector<State> states) : 
Bee( centre, getScoutConfig()["size"].toDouble(), hive, 
	getScoutConfig()["energy"]["initial"].toDouble(), 
	getScoutConfig()["speed"].toDouble()),
CFSM(states)
{}

//éxecution des actions liées à l'état courant
void onState(State current,sf::Time dt)
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



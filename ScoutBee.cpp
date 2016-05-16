#include "ScoutBee.hpp"
#include <Application.hpp>

ScoutBee::ScoutBee(Vec2d centre, Hive* hive) : 
Bee( centre, getScoutConfig()["size"].toDouble(), hive, 
	getScoutConfig()["energy"]["initial"].toDouble(), 
	getScoutConfig()["speed"].toDouble()) 
{}

//racourci pour les données de configuration
j::Value const& ScoutBee::getScoutConfig() const
{
	return getAppConfig()["simulation"]["bees"]["scout"];
}

j::Value const& ScoutBee::getConfig() const
{
	return getScoutConfig();
}




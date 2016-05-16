#include "WorkerBee.hpp"
#include <Application.hpp>

WorkerBee::WorkerBee(Vec2d centre, Hive* hive) : 
Bee( centre, getWorkerConfig()["size"].toDouble(), hive, 
	getWorkerConfig()["energy"]["initial"].toDouble(), 
	getWorkerConfig()["speed"].toDouble())
{}

//racourci pour les données de configuration
j::Value const& WorkerBee::getWorkerConfig() const
	{
		return getAppConfig()["simulation"]["bees"]["worker"];
	}

j::Value const& WorkerBee::getConfig() const
	{
		return getWorkerConfig();
	}


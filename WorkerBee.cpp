#include "WorkerBee.hpp"
#include <Application.hpp>

State const WorkerBee::IN_HIVE = createUid();
State const WorkerBee::GET_POLLEN = createUid();
State const WorkerBee::RETURN_HIVE = createUid();

//constructeur
WorkerBee::WorkerBee(Vec2d centre, Hive* hive, vector<State> states) : 
Bee( centre, getWorkerConfig()["size"].toDouble(), hive, 
	getWorkerConfig()["energy"]["initial"].toDouble(), 
	getWorkerConfig()["speed"].toDouble()),
CFSM(states)
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


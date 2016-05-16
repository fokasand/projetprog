#include "WorkerBee.hpp"
#include <Application.hpp>

State const WorkerBee::GET_POLLEN = createUid();
State const WorkerBee::RETURN_HIVE = createUid();

//constructeur
WorkerBee::WorkerBee(Vec2d centre, Hive* hive) : 
Bee( centre, getWorkerConfig()["size"].toDouble(), hive, 
	getWorkerConfig()["energy"]["initial"].toDouble(), 
	getWorkerConfig()["speed"].toDouble(),
	getAppTexture(getWorkerConfig()["texture"].toString()))
{
	setloss();
	states_.push_back(GET_POLLEN);
	states_.push_back(RETURN_HIVE);
}

//éxecution des actions liées à l'état courant
void WorkerBee::onState(State current,sf::Time dt)
{
//TODO: à coder	
}
	
//racourci pour les données de configuration
j::Value const& WorkerBee::getWorkerConfig() const
{
	return getAppConfig()["simulation"]["bees"]["worker"];
}

j::Value const& WorkerBee::getConfig() const
{
	return getWorkerConfig();
}

#include "WorkerBee.hpp"
#include "ScoutBee.hpp"
#include <Application.hpp>

State const WorkerBee::IN_HIVE = createUid();
State const WorkerBee::SEARCH_FLOWER = createUid();
State const WorkerBee::TO_HIVE = createUid();
vector<State> WorkerBee::etats_ = vector<State> (IN_HIVE,TO_HIVE);

//constructeur
WorkerBee::WorkerBee(Vec2d centre, Hive* hive) : 
Bee( etats_,centre, getWorkerConfig()["size"].toDouble(), hive, 
	getWorkerConfig()["energy"]["initial"].toDouble(), 
	getWorkerConfig()["speed"].toDouble(),
	getAppTexture(getWorkerConfig()["texture"].toString()))
{
	setTout();
}

//éxecution des actions liées à l'état courant
void WorkerBee::onState(State current,sf::Time dt)
{
//TODO: à coder	
}

void WorkerBee::onEnterState(State state)
{
	//TODO
}
	
//racourci pour les données de configuration
j::Value const& getWorkerConfig()
{
	return getAppConfig()["simulation"]["bees"]["worker"];
}

j::Value const& WorkerBee::getConfig() const
{
	return getWorkerConfig();
}

//methodes d'interaction
void WorkerBee::interact(Bee* other)
{
	other->interactWith(this);
}

void WorkerBee::interactWith(ScoutBee* scouting)
{
	
}

void WorkerBee::interactWith(WorkerBee* working)
{
	//nothing todo
}

void WorkerBee::drawOn(sf::RenderTarget& target) const 
{
	Bee::drawOn(target);
	if (isDebugOn())
	{
		Vec2d affiche (centre.x, centre.y - 25 );
		auto const text = buildText("Worker: energy "+to_nice_string(energy_), affiche , getAppFont(), 10, sf::Color::Blue); // + statestring_
		target.draw(text);
	}
    
}

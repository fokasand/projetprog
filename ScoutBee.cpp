#include "ScoutBee.hpp"
#include <Application.hpp>
#include "Env.hpp"
#include "WorkerBee.hpp"

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
	enmin_flower(getScoutConfig()["energy"]["to seek flowers"].toDouble()),
	max_share(getScoutConfig()["sharing"]["max"].toInt()),
	counter(0)
{
	setTout();
}

//éxecution des actions liées à l'état courant
void ScoutBee::onState(State current, sf::Time dt)
{
	
	if(current==IN_HIVE)
	{
		// donner l'addresse à une worker
		{
			statestring_="in_hive_sharing" + to_nice_string(counter);
		}
		
		if (energy_< enmin_hive)
		{
			statestring_= "in_hive_eat";
			eat(dt);
		}
		//passer l'adresse avec setmemory_
		if((memory_==nullptr) and (energy_>enmin_hive)) //en vrai c'est pas ça la condition mais plutot un bool d'une methode de passage
		//arrêter de la dessiner dans draw (utilser continue)
		//passer à l'état suivant (recherche d'une fleur)
		{
			statestring_=="in_hive_leaving";
			nextState();
		}
	}
	
	if(current == TO_HIVE)
	{	
		if(isPointInside(hive_->getPosition()))
		{
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
			//donner l'addresse a une worker dans le tableau //waiting list
			// switch et popback
			moveMode_=MoveMode::Rest;
		}
		
		if(state==SEARCH_FLOWER)
		{
			//effacer la mémoire on admet que la position en mémoire à étée communiquée
			memory_=nullptr;
			counter=0;
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

//méthodes permettant l'intéraction entre deux abeilles
void ScoutBee::interact(Bee* other)
{
	other->interactWith(this);
}

void ScoutBee::interactWith(ScoutBee* scouting)
{
	//nothing todo
}

void ScoutBee::interactWith(WorkerBee* working)
{
	if(counter < max_share)
	{
		working->setMemory(memory_);
		++counter;
	}
	
}

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

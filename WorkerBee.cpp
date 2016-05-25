#include "WorkerBee.hpp"
#include <Application.hpp>
#include "Env.hpp"

State const WorkerBee::IN_HIVE = createUid();
State const WorkerBee::TO_HIVE = createUid();
State const WorkerBee::GET_POLLEN = createUid();
State const WorkerBee::SEARCH_FLOWER = createUid();
vector<State> WorkerBee::etats_ = {IN_HIVE, SEARCH_FLOWER, GET_POLLEN, TO_HIVE};

//constructeur
WorkerBee::WorkerBee(Vec2d centre, Hive* hive) : 
Bee(etats_, // pour sncm
	centre, getWorkerConfig()["size"].toDouble(), hive, // pour collider
	getWorkerConfig()["energy"]["initial"].toDouble(), 
	getWorkerConfig()["speed"].toDouble(),
	getAppTexture(getWorkerConfig()["texture"].toString())),
	
	max_pollen_(getWorkerConfig()["max pollen capacity"].toDouble()),// attributs propres à la classe
	harvestStep_(getWorkerConfig()["harvest rate"].toDouble()),
	nectarStep_ (getWorkerConfig()["transfer rate"].toDouble())
{
	setTout();
	states_.push_back(GET_POLLEN);
}

void WorkerBee::onState(State current, sf::Time dt)
{
	if(current==IN_HIVE)
	{
		if (nectarbee_ > 0)
		{
			nectarbee_ -= nectarStep_;
			hive_->dropPollen(nectarStep_);
			statestring_ = "in_hive_pollen";
		}
		else
		{
			if (energy_ < enmin_hive)
			{
				eat(dt);
				statestring_ = "in_hive_eat";
			}
			else
			{
				if ( memory_ != nullptr)
				{
					if (energy_ > enmin_hive)
					{
						nextState();
					}
					statestring_="in_hive_???";
				}
				statestring_ = "in_hive_no_flower";
			}
		}
	}
	
	if(current==SEARCH_FLOWER)
	{
		if (isPointInside(*memory_))
		{
			nextState();
		}
	}
	
	if (current == GET_POLLEN)
	{
		if ( (getAppEnv().getCollidingFlower(*this) != nullptr) and (nectarbee_ < max_pollen_))
		{
		nectarbee_ += harvestStep_; // ajoute le pollen au stock de l'abeille
		getAppEnv().getCollidingFlower(*this)->takePollen(harvestStep_); // retire le pollen de la fleur
		statestring_ = "collecting_pollen";
		}
		else 
		{
			nextState();
		}
	}
	if(current==TO_HIVE)
	{
		statestring_="back_to_hive";
		if ((getAppEnv().getCollidingHive(*this)->getPosition() == target_))
		{
			nextState();
		}
	}
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

//éxecution des actions liées à l'état courant
void WorkerBee::onEnterState(State current)
{
	if(current==IN_HIVE)
	{
		//effacer la mémoire pour qu'elle puisse être remplie par un scout dans la ruche.
		memory_=nullptr;
		// ajoute l'abeille à la waiting list
		//hive_.addToList(this);
		if (hive_->getNectar() > 0)
		{
			//méthode givenectar
		}
		
		moveMode_ = Rest;
	}
	
	if(current==SEARCH_FLOWER)
	{
		statestring_ = "to_flower";
		target_ = *memory_;
		moveMode_=Targeted;
	}
	
	if(current==TO_HIVE)
	{
		//prendre la ruche comme cible de déplacment
		target_=hive_->getPosition();
		moveMode_=Targeted;
	}
}


void WorkerBee::learnFlowerLocation(Vec2d flowerPosition)
{
	memory_ = &flowerPosition;
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






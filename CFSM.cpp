#include "CFSM.hpp"

//constructeur
CFSM::CFSM(std::vector<State> states)
: states_(states), current_(0)
{}
	
//rend l'état courant
State  CFSM::getState()
{
	return current_;
}
	
//passe à l'état suivant
void CFSM::nextState()
{
	//passer à l'état suivant si fin de la liste pas encore attent
	if(current_< current_.size())
	{
		++current_;
	} else
	{
		//recommence au début de la liste des états
		current_=0;
	}
}

void CFSM::onEnterState(State state)
{
	
}
//actions à l'état courant
void CFSM::onState(State current,sf::Time dt)
{
	
}

void CFSM::action(sf::Time dt)
{
	
}
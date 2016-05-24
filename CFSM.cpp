#include "CFSM.hpp"

//constructeur
CFSM::CFSM(std::vector<State> states)
: states_(states), current_(0)
{}

CFSM::~CFSM()=default;
//rend l'état courant
State  CFSM::getState()
{
	return current_;
}
	
//passe à l'état suivant
void CFSM::nextState()
{
	//passer à l'état suivant si fin de la liste pas encore attent
	if(current_< states_.size())
	{
		++current_;
	} else
	{
		//recommence au début de la liste des états
		current_=0;
	}
	onEnterState(current_);
}

//actions à l'état courant


void CFSM::action(sf::Time dt)
{
	onState(current_, dt);
}


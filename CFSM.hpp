#ifndef CFSM_H
#define CFMS_H
#include "Utility/Utility.hpp"

typedef Uid State;
class CFSM {

protected:
//constructeur
	CFSM(std::vector<State>);
//rend létat courant
	State getState();
	
//passe à l'état suivant
	void nextState();
	//ne fait rien par défaut ?
	virtual void onEnterState(State state) =0;

//actions à l'état courant
	void onState(State current,sf::Time dt);
	void action(sf::Time dt);

private:	
	std::vector<State> states_;
	size_t current_;
	
	
};
#endif

#ifndef SFCM_H
#define SFCM_H
#include "Utility/Utility.hpp"

typedef Uid State;
class CFSM {

protected:
//constructeur
	CFSM(std::vector<State>);
	CFSM() =default;
//retablir destructeur par défaut
	~CFSM();
//rend létat courant
	State getState();
	
//passe à l'état suivant
	void nextState();
	//ne fait rien par défaut ?
//execute les actions de transition
	virtual void onEnterState(State state);

//actions à l'état courant
	virtual void onState(State current,sf::Time dt) = 0;
	void action(sf::Time dt);

protected:	
	std::vector<State> states_;
	size_t current_;
	
	
};
#endif

#ifndef CFSM_H
#define CFMS_H
#include "Utility/Utility.hpp"

typedef Uid State;

class CFSM {
public:
//rend létat courant
	State getState();

protected:
//constructeur
	CFSM(std::vector<State>&);
	CFSM() =default;
//retablir destructeur par défaut
	virtual ~CFSM();
	
//passe à l'état suivant
	void nextState();

//execute les actions de transition
	virtual void onEnterState(State state) = 0;

//actions à l'état courant
	virtual void onState(State current,sf::Time dt) = 0;

//appelle onState en utilisant l'état courant de CFSM
	void action(sf::Time dt);

private: 
//tableau contenant les états possibles
	std::vector<State> states_;

//indice correspondant à l'état courant
	size_t current_;
};
#endif

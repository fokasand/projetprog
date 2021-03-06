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
	//ne fait rien par défaut ?
//execute les actions de transition
	virtual void onEnterState(State state) = 0;

//actions à l'état courant
	virtual void onState(State current,sf::Time dt) = 0;
	void action(sf::Time dt);

protected:	
	std::vector<State> states_;
	size_t current_;
	
	
};
#endif

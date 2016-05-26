#ifndef WORKERBEE_H
#define WORKERBEE_H
#include "Bee.hpp"

class WorkerBee : public Bee
{
public:
	//constructeur
	WorkerBee(Vec2d centre, Hive* hive);
	//racourci pour les données de configuration

	j::Value const& getConfig() const override;
	
	//éxecution des actions liées à l'état courant
	void onState(State current,sf::Time dt) override;
	void onEnterState(State state) override;
	
	//méthodes permettant l'intéraction entre deux abeilles
	virtual void interact(Bee* other) override;
	virtual void interactWith(ScoutBee* scouting) override;
	virtual void interactWith(WorkerBee* working) override;
	
private:
	static State const SEARCH_FLOWER;
	static const State IN_HIVE;
	static const State TO_HIVE;
	//tableau rassemblant les états communs à touts types d'abeilles	
	static std::vector<State> etats_ ;
	virtual void drawOn(sf::RenderTarget& targetWindow) const override;
};

j::Value const& getWorkerConfig();

#endif
	

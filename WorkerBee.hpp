#ifndef WORKERBEE_H
#define WORKERBEE_H
#include "Bee.hpp"

class WorkerBee : public Bee
{
public:
	//constructeur
	WorkerBee(Vec2d centre, Hive* hive);
	//racourci pour les données de configuration
	j::Value const& getWorkerConfig() const;

	j::Value const& getConfig() const;
	
	//éxecution des actions liées à l'état courant
	void onState(State current,sf::Time dt) override;
	void onEnterState(State current);
	

	
	//méthodes permettant l'intéraction entre deux abeilles
	virtual void interact(Bee* other) override;
	virtual void interactWith(ScoutBee* scouting) override;
	virtual void interactWith(WorkerBee* working) override;
	
private:
	
	static const State GET_POLLEN;
	static const State SEARCH_FLOWER;
	static const State TO_HIVE;
	static const State IN_HIVE;
	static vector<State> etats_;
	const double max_pollen_;
	const double harvestStep_;
	const double nectarStep_;

	double nectarbee_;

	virtual void drawOn(sf::RenderTarget& targetWindow) const;
};

#endif

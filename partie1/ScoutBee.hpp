#ifndef SCOUTBEE_H
#define SCOUTBEE_H
#include "Bee.hpp"

class ScoutBee : public Bee
{
public:
	//constructeur
	ScoutBee(Vec2d centre, Hive* hive);
	
	
	//éxecution des actions liées à l'état courant
	void onState(State current,sf::Time dt) override;
	void onEnterState(State state) override;
	
	//racourci pour les données de configuration
	virtual j::Value const& getConfig() const override;
	
	//méthodes permettant l'intéraction entre deux abeilles
	virtual void interact(Bee* other) override;
	virtual void interactWith(ScoutBee* scouting) override;
	virtual void interactWith(WorkerBee* working) override;
	
private:
	
	static const State IN_HIVE;
	static const State SEARCH_FLOWER;
	static const State TO_HIVE;
	static std::vector<State> etats_ ;
	
	//energie minimale pour chercher une fleur
	double const enmin_flower;
	
	//seuil pour la partage de la mémoire
	int const max_share;
	
	//compteur pour le nombre de partages
	int counter;
	
	//accès au fichier de configuration
	static j::Value const& getScoutConfig();
	
	virtual void drawOn(sf::RenderTarget& targetWindow) const override;
	
};

#endif

#ifndef SCOUTBEE_H
#define SCOUTBEE_H
#include "Bee.hpp"
#include "Hive.hpp"

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
	
private:
	static State const SEARCH_FLOWER;

	double const enmin_flower;
	
	static j::Value const& getScoutConfig();

	
};

#endif

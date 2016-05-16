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
	
	//racourci pour les données de configuration
	j::Value const& getScoutConfig() const;

	j::Value const& getConfig() const;
	
private:
	static const State IN_HIVE;	
};

#endif

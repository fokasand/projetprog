#ifndef WORKERBEE_H
#define WORKERBEE_H
#include "Bee.hpp"
#include "Hive.hpp"

class WorkerBee : public Bee 
{
public:
	//constructeur
	WorkerBee(Vec2d centre, Hive* hive);
	//racourci pour les données de configuration
	j::Value const& getWorkerConfig() const;

	j::Value const& getConfig() const;
private:

	
};

#endif

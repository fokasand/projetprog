#ifndef WORKERBEE_H
#define WORKERBEE_H
#include "Bee.hpp"

class WorkerBee : public Bee 
{
public:
	//constructeur de workerbee qui ne prend aucun paramètres, va seulement les chercher dans le fichier de configuration.
	/*WorkerBee();
	//racourci pour les données de configuration
	j::Value Bee::getWorkerConfig() const 
	{
		return getAppConfig()["simulation"]["bees"]["worker"];
	}

	j::Value Bee::getConfig() const
	{
		return getWorkerConfig();
	}
	*/ 
private:

	
};

#endif

#ifndef ENV_H
#define ENV_H
#include "World.hpp"

class Env {
	
	public:
	//constucteur
	Env();
	
	//attribus
	World world_;
	
	//regeneration de l'environnement
	void reset();
	
	//evolution de l'environnement sur un temps dt
	void update(sf::Time dt);
	
	//dessin de l'environnement
	void drawOn(sf::RenderTarget& target);
	
	//génération de l'environnement à partir d'un fichier
	void loadWorldFromFile();
	
	//sauvegarder l'environnement dans un ficher
	void saveWorldToFile();	
	
};

#endif

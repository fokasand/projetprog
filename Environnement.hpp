#ifndef ENV_H
#define ENV_H
#include "World.hpp"
#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "Flower.hpp"

//racourci données de configuration pour env
j::Value getEnv();

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
	
	// ajout de fleur dans env
	void addFlowerAt (Vec2d p);
	
	//bloquer copie end Ev
	Env(const Env& e)= delete;
	
private:
	//collection de fleurs
	std::vector <std::unique_ptr<Flower>> flowers_;
	

};

#endif

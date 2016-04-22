#ifndef FLOWER_H
#define FLOWER_H
#include "Collider.hpp"
#include "Utility/Utility.hpp"
#include <SFML/Graphics.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>

j::Value getFlower(); // raccourci pour acceder aux données

class Flower : public Collider {
	
	public :
	//constructeur de Flower avec 3 paramètres
	Flower (const Vec2d& c, const double& r, double qpollen);
	
	//retirer du pollen tant qu'il est disponible
	double takePollen (double take);
	
	//dessiner les fleurs
	void drawOn(sf::RenderTarget& target) const;
	
	protected : 
	
	double pollen;
	// indice maximal de texture -> creation d'un int pour pouvoir utiliser uniform
	int max; 
	//initialisation de la textre de la fleur, ne change plus
	sf::Texture const texture;
	
};

#endif

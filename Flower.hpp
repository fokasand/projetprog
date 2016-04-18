#ifndef FLOWER_H
#define FLOWER_H
#include "Collider.hpp"
#include "Utility/Utility.hpp"
#include <SFML/Graphics.hpp> // a include ici ou dans cpp ?


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
	sf::Texture const& texture;
	
	
};

#endif

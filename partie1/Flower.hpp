#ifndef FLOWER_H
#define FLOWER_H
#include "Collider.hpp"
#include "Utility/Utility.hpp"
#include <SFML/Graphics.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

j::Value getFlower(); // raccourci pour acceder aux données

class Flower : public Collider, public Drawable, public Updatable{
	
	public :
	//constructeur de Flower avec 3 paramètres
	Flower (const Vec2d& c, const double& r, double qpollen);
	
	//retirer du pollen tant qu'il est disponible
	double takePollen (double take);
	
	//dessiner les fleurs
	virtual void drawOn(sf::RenderTarget& target) const override;
	
	//mettre a jour la fleur (quantité de pollen)
	void update(sf::Time dt);
	
	 // utilisé dans Env pour tuer les fleurs.
	double getPollen() const;

	protected : 
	
	double pollen;
	// indice maximal de texture -> creation d'un int pour pouvoir utiliser uniform
	int max; 
	//initialisation de la textre de la fleur, ne change plus
	sf::Texture const texture;
	
	const double split_;
	
	const double threshold_;

};

#endif

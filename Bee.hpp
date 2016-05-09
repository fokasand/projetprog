
#ifndef BEE_H
#define BEE_H
#include "Collider.hpp"
#include "Hive.hpp"
#include <Application.hpp>
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

class Bee: public Collider,public Drawable, public Updatable {
	
protected:
	Vec2d speed_;
	double energy_;
	Hive* hive_;
	
	
//constructeur
		Bee(Vec2d centre,
			double rayon,
			 Hive* hive,
			 double energy, double amplitude);
		

//morte si energie nulle
bool isDead();

//déplacement : calcule nouvelles positions et vitesse
void move(sf::Time dt);
	
};

#endif

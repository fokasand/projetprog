#ifndef BEE_H
#define BEE_H
#include "Collider.hpp"
#include "Hive.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

class Bee: public Collider,public Drawable, public Updatable {

public:
//constructeur		
	Bee(Vec2d centre,
		double rayon,
		Hive* hive,
		double energy, double amplitude);

//destructeur
		~Bee();

//morte si energie nulle
	bool isDead();

//déplacement : calcule nouvelles positions et vitesse
	void move(sf::Time dt);


//dessin des abeilles
void drawOn(sf::RenderTarget& targetWindow) const override;

//mise a jour des abeilles
void update(sf::Time dt);

// retourne le jvalue dans classe pour pouvoir le redefinir
virtual j::Value getConfig() const;

protected:
	Hive* hive_;
	Vec2d speed_;
	double energy_;
	
};

#endif



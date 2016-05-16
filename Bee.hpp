#ifndef BEE_H
#define BEE_H
#include "Collider.hpp"
#include "Hive.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

class Bee: public Collider, public Drawable, public Updatable 
{
	
protected:
	Hive* hive_;
	double energy_;
	Vec2d speed_;
	double prob;
	double alpha_max;
	
	sf::Texture texture_;
		
public:	
	//constructeur
	Bee(Vec2d centre,
		double rayon,
		 Hive* hive,
		 double energy, double amplitude,
		 sf::Texture texture);

	//morte si energie nulle
	bool isDead();

	//déplacement : calcule nouvelles positions et vitesse
	void update(sf::Time dt);
	
	void drawOn(sf::RenderTarget& targetWindow) const;
	
	void randomMove(sf::Time dt);
	
	// retourne le jvalue utilisé pour raccourcir
	virtual j::Value const& getConfig() const = 0;
	j::Value const& getBeeConfig() const; // utilisé pour le constructeur

};

#endif


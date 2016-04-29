#ifndef HIVE_H
#define HIVE_H
#include "Collider.hpp"
#include <Interface/Updatable.hpp>
#include <SFML/Graphics.hpp>
#include <Application.hpp>

//racourci pour les données de configuration
j::Value getHive()

class Hive: public Collider, public Updatable, public Drawable {
public:
	
	//constructeur
	Hive(const Vec2d& c,const double& r);
	
	//ajouter una abeille à la ruche
	void addBee;
	
	//faire évoluer les abeilles
	void update(sf::Time dt);
	
	void drawOn(sf::RenderTarget& targetWindow) const override;
	
	//ajouter du nectar à la ruche
	void dropPollen(double qte);
	
	//retirer du nectar de la ruche
	void takeNectar(double qte);
	
private:
	double nectar_;
	vector <Bee*> bees_;
	sf::Texture const texture;
	
};


#endif

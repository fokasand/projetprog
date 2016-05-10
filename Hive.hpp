#ifndef HIVE_H
#define HIVE_H
#include "Collider.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

class Bee;

class Hive : public Collider, public Drawable, public Updatable{
public:
	
	//constructeur
	Hive() = default;
	Hive(const Vec2d& c);
	
	//bloquer copie
	Hive(Hive const& other) = delete;
	void operator =(Hive const& other) = delete;
	
	//destructeur
	~Hive();
	
	//faire évoluer les abeilles
	void update(sf::Time dt);
	
	void drawOn(sf::RenderTarget& targetWindow) const override;
	
	//ajouter du nectar à la ruche
	void dropPollen(double qte);
	
	//retirer du nectar de la ruche
	void takeNectar(double qte);
	
	//ajouter une abeille à la ruche
	void addBee();
	
	//efface les abeilles mortes
	void killBee();
	
	
private:
	double nectar_;
	std::vector <Bee*> bees_;
	sf::Texture const texture;
	//efface les abeilles et nettoie le tableau
	void clearBees();
	
};

//racourci pour les données de configuration
j::Value getHive();

#endif

#ifndef HIVE_H
#define HIVE_H
#include "Collider.hpp"
#include <Application.hpp>
#include "Bee.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

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
	
	//ajouter una abeille à la ruche
	void addBee();
	
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

//racourci pour les données de configuration
j::Value getHive();

#endif

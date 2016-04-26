#ifndef ENV_H
#define ENV_H
#include "World.hpp"
#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "Flower.hpp"
#include "FlowerGen.hpp"
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

//racourci données de configuration pour env
j::Value getEnv();

class Env: public Drawable, public Updatable {
	
public:
	//constucteur
	Env();
	// destructeur
	~Env() 
	{
		for (size_t i(0); i < flowers_.size() ; ++i)
		{
			flowers_[i].reset();
		}
		flowers_.clear();
	}
	//attribus
	World world_;
	
	//regeneration de l'environnement
	void reset();
	
	//evolution de l'environnement sur un temps dt
	virtual void update(sf::Time dt) override;
	
	//dessin de l'environnement
	virtual void drawOn(sf::RenderTarget& target) const override;
	
	//génération de l'environnement à partir d'un fichier
	void loadWorldFromFile();
	
	//sauvegarder l'environnement dans un ficher
	void saveWorldToFile();	
	
	// ajout de fleur dans env
	bool addFlowerAt (Vec2d p);
	
	//bloquer copie end Ev
	Env(const Env& e)= delete;
	
	//afficher anneau
	void drawFlowerZone(sf::RenderTarget& target,Vec2d const& position);
	
	//tue les fleurs
	void killFlower();
private:
	//collection de fleurs
	std::vector <std::unique_ptr<Flower>> flowers_;
	
	FlowerGenerator fgen_;
	
	sf::Time t;

};

#endif

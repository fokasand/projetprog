#ifndef ENV_H
#define ENV_H
#include "World.hpp"
#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "Flower.hpp"
#include "FlowerGen.hpp"
#include "Hive.hpp"
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

//racourci données de configuration pour env
j::Value const& getEnv();

class Env: public Drawable, public Updatable
{

public:
    //constucteur
    Env();
    // destructeur
    ~Env();
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

    //tue les fleurs qui n'ont plus d'eéergie
    void killFlower();
    
	//ajouter une ruche
	bool addHiveAt(const Vec2d& position);
	
	//rend un pointeur la ruche en collision avec l'argument
	Hive* getCollidingHive(const Collider& body);
	
	//rend un pointeur sur la fleur en collision avec l'argument
	Flower* getCollidingFlower(const Collider& body);
	
	sf::Color couleur(const Vec2d& position, double rayon); // aide partie bonus
	void drawHiveableZone(sf::RenderTarget& target, Vec2d const& position);

private:
    //collection de fleurs
    std::vector <Flower*> flowers_;

    FlowerGenerator fgen_;

    sf::Time t;

    //vire les fleurs.
    void clearFlowers();
    
    //collection ruches
    vector<Hive*> hives_;
    
    //effacer les ruches
    void clearHives();
    
    //nombre maximal de fleurs;
    unsigned int maxFlower;
};

#endif

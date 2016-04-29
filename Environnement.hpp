#ifndef ENV_H
#define ENV_H
#include "World.hpp"
#include "Flower.hpp"
#include "FlowerGen.hpp"
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

//racourci données de configuration pour env
j::Value getEnv();

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

    //tue une fleur
    void killFlower();

	double howhumid(Vec2d const& p);
private:
    //collection de fleurs
    std::vector <Flower*> flowers_;

    FlowerGenerator fgen_;

    sf::Time t;

    //vire les fleurs.
    void clearFlowers();
};

#endif

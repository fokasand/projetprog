#include "Env.hpp"
#include <Random/Random.hpp>

//racourci données de configuration pour env
j::Value getEnv()
{
    return getAppConfig()["simulation"]["env"];
}

//constructeur par défaut
Env::Env()
{
    try {
        loadWorldFromFile();
    } catch(std::runtime_error error) {
        std::cerr << "fichier non ouvert : regeneration aléatoire" << std::endl;
        world_.reset(true);
    }
}

//regeneration de l'environnement
void Env::reset()
{
    fgen_.reset();

    world_.reset(true);
    clearFlowers();
    clearHives();

}

void Env::killFlower()
{
    for (size_t i(0); i < flowers_.size() ; ++i) {
        if (flowers_[i]->getPollen() <= 0) {
            delete flowers_[i];
            flowers_[i]= nullptr;
        }
    }

    flowers_.erase(std::remove(flowers_.begin(), flowers_.end(), nullptr), flowers_.end());
}

//evolution de l'environnement sur un temps dt
void Env::update(sf::Time dt)
{
	vector<Flower*> copie(flowers_);
	
    for (auto& fleur : copie) {
        fleur->update(dt);
    }
    fgen_.update(dt); // fait à l'étape 3, réponse question.
    
    for (size_t i(0); i < hives_.size() ; ++i) {
        hives_[i]->update(dt);
    }
}

//dessin de l'environnement
void Env::drawOn(sf::RenderTarget& target) const
{
	cerr << "in env size of hives is " << hives_.size() << std::endl;
    //dessin du terrain
    world_.drawOn(target);

    //dessin de toutes les fleurs une par une
    for (size_t i(0); i < flowers_.size() ; ++i) {
        flowers_[i]->drawOn(target);
    }
    // dessin de toutes les ruches
     for (size_t i(0); i < hives_.size() ; ++i) {
        hives_[i]->drawOn(target);
    }

}

// ajout de fleur dans env
bool Env::addFlowerAt (Vec2d p)
{
    //la cellule correspondant à p doit être de l'herbe
    unsigned int maxFlower (getEnv()["max flowers"].toInt());

    if (world_.isGrowable(p) and flowers_.size()< maxFlower) {
        double min (getEnv()["initial"]["flower"]["nectar"]["min"].toDouble());
        double max (getEnv()["initial"]["flower"]["nectar"]["max"].toDouble());
        double r (getEnv()["initial"]["flower"]["size"]["manual"].toDouble());
        flowers_.push_back(new Flower(p,r,uniform(min,max)));
        return true;
    }
    return false;
}

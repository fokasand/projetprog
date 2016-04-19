#include "Env.hpp"

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
		}
	catch(std::runtime_error error)
	{
		std::cerr << "fichier non ouvert : regeneration aléatoire" << std::endl;
		world_.reset(true);
	}
}

//regeneration de l'environnement
void Env::reset()
{
	world_.reset(true);
}

//evolution de l'environnement sur un temps dt
void Env::update(sf::Time dt)
{

}

//dessin de l'environnement
void Env::drawOn(sf::RenderTarget& target)
{
	world_.drawOn(target);
}

// ajout de fleur dans env
void Env::addFlowerAt (Vec2d p)
{
	//la cellule correspondant à p doit être de l'herbe
	if (world_.World::isGrowable(p) and flowers_.size()< getEnv()["max flowers"].toInt())
	{
		double min (getEnv()["initial"]["flower"]["nectar"]["min"].toDouble());
		double max (getEnv()["initial"]["flower"]["nectar"]["max"].toDouble());
		double r (getEnv()["initial"]["flower"]["size"]["manual"].toDouble());
		std::unique_ptr<Flower> p1 (new Flower(p,r,uniform(min,max)));
		flowers_.push_back(p1);
		
	}

}
//génération d'un environnement à partir d'un fichier
void Env::loadWorldFromFile()
{
	world_.loadFromFile();
}

// sauvegarde 
void Env::saveWorldToFile()
{
	world_.saveToFile();
}


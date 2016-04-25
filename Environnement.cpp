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
	fgen_.reset();
	
	world_.reset(true);
	for (size_t i(0); i < flowers_.size() ; ++i)
	{
		flowers_[i].reset();
	}
	flowers_.clear();
	
}

void Env::killFlower()
{
	for (size_t i(0); i < flowers_.size() ; ++i)
	{
		if (flowers_[i]->getPollen() <= 0)
		{
			std::cout << "bonjour" << std::endl;
			flowers_[i].reset();
			std::cout << "toujour" << std::endl;
			flowers_[i]= nullptr;
			std::cout << "ok" << std::endl;
			flowers_.erase(std::remove(flowers_.begin(), flowers_.end(), nullptr), flowers_.end());
		}
	}

	flowers_.erase(std::remove(flowers_.begin(), flowers_.end(), nullptr), flowers_.end());
}

//evolution de l'environnement sur un temps dt
void Env::update(sf::Time dt)
{
	for (auto& fleur : flowers_)
	{
		fleur->update(dt);
	}
	fgen_.update(dt); // fait à l'étape 3, réponse question.
}

//dessin de l'environnement
void Env::drawOn(sf::RenderTarget& target)
{
	//dessin du terrain
	world_.drawOn(target);
	
	//dessin de toutes les fleurs une par une
	for (size_t i(0); i < flowers_.size() ; ++i)
	{
		flowers_[i]->drawOn(target); 
	}
	
}

// ajout de fleur dans env
bool Env::addFlowerAt (Vec2d p)
{
	//la cellule correspondant à p doit être de l'herbe
	unsigned int maxFlower (getEnv()["max flowers"].toInt());
	if (world_.isGrowable(p) and flowers_.size()< maxFlower )
	{
		double min (getEnv()["initial"]["flower"]["nectar"]["min"].toDouble());
		double max (getEnv()["initial"]["flower"]["nectar"]["max"].toDouble());
		double r (getEnv()["initial"]["flower"]["size"]["manual"].toDouble());
		flowers_.push_back(std::unique_ptr<Flower> (new Flower(p,r,uniform(min,max))));	
		
		return true;
	}
	return false;
}

//afficher anneau
void Env::drawFlowerZone(sf::RenderTarget& target,Vec2d const& position)
{
	sf::Color color = (world_.isGrowable(position)) ? sf::Color::Green : sf::Color::Red;
	double size (getEnv()["initial"]["flower"]["size"]["manual"].toDouble());
	auto shape = buildAnnulus(position, size, color, 0.5);
	target.draw(shape);
	
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

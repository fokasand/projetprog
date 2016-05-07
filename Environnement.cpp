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
    for (auto& fleur : flowers_) {
        fleur->update(dt);
    }
    fgen_.update(dt); // fait à l'étape 3, réponse question.
}

//dessin de l'environnement
void Env::drawOn(sf::RenderTarget& target) const
{
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



//afficher anneau
void Env::drawFlowerZone(sf::RenderTarget& target,Vec2d const& position)
{
    sf::Color color = (world_.isGrowable(position)) ? sf::Color::Green : sf::Color::Red; // on considère que la position ne sortira pas de la grille.
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

void Env::clearFlowers()
{
    for (size_t i(0); i < flowers_.size() ; ++i) {
        delete flowers_[i];
        flowers_[i]= nullptr;
    }
    flowers_.clear();
}

void Env::clearHives()
{
    for (size_t i(0); i < hives_.size() ; ++i) {
        delete hives_[i];
        hives_[i]= nullptr;
    }
    hives_.clear();
}
//destructeur de Env
Env::~Env()
{
    clearFlowers(); 
}

//ajouter une ruche
bool Env::addHiveAt(const Vec2d& position)
{
	//creer la ruche à tester
	Hive hive (position);

	//verifier que la ruche n'est pas en collision 
	// avec un autre element
	// verifier toutes les fleurs
	for (size_t i(0); i < flowers_.size() ; ++i)
	{
		if(hive.isColliding(*flowers_[i]))
		{
			return false;
		}
	}
	
	//verifier toutes les ruches
	for (size_t i(0); i < hives_.size() ; ++i)
	{
		Collider test (hives_[i]->getPosition(),
		getEnv()["initial"]["hive"]["size"]["manual"].toDouble()*
		getEnv()["initial"]["hive"]["hiveable factor"].toDouble());
		
		if(hive.isColliding(test))
		{
			return false;
		}
	}
	
	//si n'est en collision avec rien
	//peut être ajoutée
	hives_.push_back(new Hive (position));
	return true;
}

//rend la ruche en collision avec l'argument 
//(la premiere dans la teableau qui soit en collision)
Hive* Env::getCollidingHive(const Collider& body)
{
	for (size_t i(0); i < hives_.size() ; ++i)
	{
		Collider test(hives_[i]->getPosition(),
		getEnv()["initial"]["hive"]["size"]["manual"].toDouble()*
		getEnv()["initial"]["hive"]["hiveable factor"].toDouble());
		
		if(body.isColliding(test))
		{
			return new Hive (hives_[i]->getPosition());
		}
	}
	return nullptr;
}

//rend la fleur en collision avec l'argument
Flower* Env::getCollidingFlower(const Collider& body)
{
	for (size_t i(0); i < flowers_.size() ; ++i)
	{
		if(body.isColliding(*flowers_[i]))
		{
			return flowers_[i];
		}
	}
	return nullptr;		
}

 //partie bonus:
void Env::drawHiveableZone(sf::RenderTarget& target, Vec2d const& position) 
{
	double rayon = getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble();
	Vec2d topLeft = {position.x-rayon,position.y-rayon};
	Vec2d bottomRight = {position.x+rayon,position.y+rayon};
	clamping_(topLeft);
	clamping_(bottomRight);
	Vec2d worldSize = getApp().getWorldSize();
	double width = worldSize.x;
    double height= worldSize.y;
    sf::Color color(couleur(position, rayon));
	if ((topLeft.x < bottomRight.x) and (topLeft.y < bottomRight.y)) // cas 1 décrit dans indexes for rect
	{
	sf::RectangleShape shape(buildRectangle({ position.x-rayon, position.y+rayon }, { position.x+rayon, position.y-rayon }, color, 5.0));
	target.draw(shape);
	}
	if ((topLeft.x > bottomRight.x) and ( topLeft.y > bottomRight.y))// cas 2
	{
		sf::RectangleShape shape1(buildRectangle({ 0.0,0.0 }, bottomRight, color, 5.0)); // top left shape
		sf::RectangleShape shape2(buildRectangle({ 0.0, topLeft.y }, {bottomRight.x, height}, color, 5.0)); // bottom left
		sf::RectangleShape shape3(buildRectangle( topLeft, {width, height }, color, 5.0)); // bottom right shape
		sf::RectangleShape shape4(buildRectangle({topLeft.x, 0.0}, { width, bottomRight.y }, color, 5.0)); // top right
		target.draw(shape1);
		target.draw(shape2);
		target.draw(shape3);
		target.draw(shape4);
	}

	if ((topLeft.x > bottomRight.x) and (topLeft.y < bottomRight.y)) // cas 3 
	{
		sf::RectangleShape shape1(buildRectangle({ 0.0, topLeft.y }, bottomRight, color, 5.0)); // left shape
		sf::RectangleShape shape2(buildRectangle(topLeft, { width, bottomRight.y }, color, 5.0));// right shape
		target.draw(shape1);
		target.draw(shape2);
	}
	if ((topLeft.x < bottomRight.x) and (topLeft.y > bottomRight.y)) // cas 4
	{
		sf::RectangleShape shape1(buildRectangle({ topLeft.x, 0.0}, bottomRight, color, 5.0)); // top shape
		sf::RectangleShape shape2(buildRectangle(topLeft, { bottomRight.x, height }, color, 5.0)); // bottom shape
		target.draw(shape1);
		target.draw(shape2);
	}
}

// on trouve maintenant la couleur du rectangle:
sf::Color Env:: couleur(const Vec2d& position, double rayon)
{
	Collider c ( position, rayon);
	if (world_.isHiveable(position, rayon)) // le carré ne contient que de l'herbe
	{
		if ((getCollidingFlower(c) == nullptr) and (getCollidingHive(c) == nullptr))
		{
			return sf::Color::Green;
		}
		else
		{
			return sf::Color::Blue; // la ruche sera quand même créée mais elle collide une fleur ou une autre ruche
		}
	}
	else
	{
	return sf::Color::Red;
	}
}



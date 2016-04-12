#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "Env.hpp"

//constructeur par défaut
Env::Env()
{
	try {
		std::cerr << "loadWorldFromFile" << std::endl;
		loadWorldFromFile();
		
		}
	catch(std::runtime_error error)
	{
		std::cerr << "regeneration aléatoire" << std::endl;
		world_.reset(true);
	}
}

//regeneration de l'environnement
void Env::reset()
{
	std::cerr << "env reset" << std::endl;
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

//génération d'un environnement à partir d'un fichier
void Env::loadWorldFromFile()
{
	world_.loadFromFile();
}

void Env::saveWorldToFile()
{
	world_.saveToFile();
}


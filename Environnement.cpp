
#include <Application.hpp>
#include "World.hpp"
#include <SFML/Graphics.hpp>
#include "Environnement.hpp"

	//regeneration de l'environnement
	void Environnement::reset()
	{
		World::reset();
	}
	
	//evolution de l'environnement sur un temps dt
	void Environnement::update(sf::Time dt)
	{
		World::updateCache();
	}
	
	//dessin de l'environnement
	void Environnement::drawOn(sf::RenderTarget& target)
	{
		World::drawOn();
	}

#include <Application.hpp>
#include "World.hpp"
#include <SFML/Graphics.hpp>
#include "Environnement.hpp"

	//regeneration de l'environnement
	void Environnement::reset()
	{
		World_.World::reset(true);
	}
	
	//evolution de l'environnement sur un temps dt
	void Environnement::update(sf::Time dt)
	{
		World_.World::updateCache();
	}
	
	//dessin de l'environnement
	void Environnement::drawOn(sf::RenderTarget& target)
	{
		World_.World::drawOn(target);
	}

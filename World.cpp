#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <iostream>

void World::drawOn(sf::RenderTarget& target)
{
	sf::Sprite cache(renderingCache_.getTexture());
	target.draw(cache);
}


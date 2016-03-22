#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include <iostream>
#include "Utility/Vertex.hpp"

//initialisation ensemble de textures
void World::reloadCacheTructure()

{	//vector contenant l'ensemble de sommets
	vector<sf::Vertex> Vertexes (generateVertexes(getTerrain()["textures"],nbCell ,cellSize));
	
	//initialisation à tous les sommets de la grille
	grassVertexes_=Vertexes;
	waterVertexes_=Vertexes;
	rockVertexes_=Vertexes;
	
	//initialisation de la texture
	renderingCache_.create(nbCell*cellSize, nbCell*cellSize);
}

void World::drawOn(sf::RenderTarget& target)
{
	sf::Sprite cache(renderingCache_.getTexture());
	target.draw(cache);
}

j::Value getTerrain()
{
	return (getAppConfig()["simulation"]["world"]);
}
 
void World::reloadConfig()
{
	nbCells = getTerrain()["cells"].toInt();
	cellSize = getTerrain()["size"].toDouble();
	
	cells_ = std::vector<Kind> (nbCells*nbCells, Kind::roche);
}

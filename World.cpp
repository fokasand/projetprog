#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include <iostream>
#include "Utility/Vertex.hpp"

//initialisation ensemble de textures
void World::reloadCacheStructure()

{	//vector contenant l'ensemble de sommets
	std::vector<sf::Vertex> Vertexes (generateVertexes(getTerrain()["textures"],nbCells ,cellSize));
	
	//initialisation à tous les sommets de la grille
	grassVertexes_=Vertexes;
	waterVertexes_=Vertexes;
	rockVertexes_=Vertexes;
	
	//initialisation de la texture
	renderingCache_.create(nbCells*cellSize, nbCells*cellSize);
}

//fonction draw
void World::drawOn(sf::RenderTarget& target)
{
	sf::Sprite cache(renderingCache_.getTexture());
	target.draw(cache);
}

//mettre a jour rendering_Cache
void World::updateCache()
{
	renderingCache_.clear();
	sf::RenderStates rs;
  rs.texture = &getAppTexture(getTerrain()["textures"]["rock"].toString()); // ici pour la texture liée à la roche
  renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs);
	renderingCache_.display();
}

j::Value getTerrain()
{
	return getAppConfig()["simulation"]["world"];
}

void World::reset(bool regenerate)
{
		if (regenerate == 1)
		{
			// ajouter regeneration aléatoire du terrain
			regenerate = 0;
		}
		if (regenerate == 0)
		{
			reloadConfig();
			reloadCacheStructure();
			updateCache();
		}
}


void World::reloadConfig()
{
	nbCells = getTerrain()["cells"].toInt();
	cellSize = getTerrain()["size"].toDouble() / nbCells;
	
	cells_ = std::vector<Kind> (nbCells*nbCells, Kind::roche);
}

void World::loadFromFile()
{
	//getApp().getResPath(); // pourquoi on en a besoin?? je n'en ai pas beosin je crois ici.
	
	std::ifstream in;
	in.open(getTerrain()["file"].toString());
	if (in.fail())
	{
		throw std::runtime_error("AIEAIEAIEAIE");
	}
	else
	{
			in >> nbCells;
			std::cout << "nbcells=" << nbCells;
			in >> cellSize;
			std::cout << "cellsize=" << cellSize;
			for (unsigned int i (0); i < cells_.size() ; ++i) // pas sur de taille
			{
			short var;
			Kind type;
			in >> var;
			type = static_cast<Kind>(var);
			cells_[i] = type;
			}
	}
	
}

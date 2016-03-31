#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include <iostream>
#include <fstream>
#include "Utility/Vertex.hpp"

//initialisation ensemble de textures
void World::reloadCacheStructure()

{	//vector contenant l'ensemble de sommets
	std::vector<sf::Vertex> Vertexes (generateVertexes(getTerrain()["textures"],nbCells_ ,cellSize_));
	
	//initialisation à tous les sommets de la grille
	grassVertexes_=Vertexes;
	waterVertexes_=Vertexes;
	rockVertexes_=Vertexes;
	
	//initialisation de la texture
	renderingCache_.create(nbCells_*cellSize_, nbCells_*cellSize_);
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
	
	sf::RenderStates rseau;
	sf::RenderStates rsherbe;
	sf::RenderStates rsroche;
	rseau.texture = &getAppTexture(getTerrain()["textures"]["water"].toString()); // ici pour la texture liée à la eau
	rsherbe.texture = &getAppTexture(getTerrain()["textures"]["rock"].toString()); // ici pour la texture liée à la roche
	rsroche.texture = &getAppTexture(getTerrain()["textures"]["grass"].toString()); // ici pour la texture liée à l'herbe
	
	int y_coord ;
	for(size_t j(0); j<cells_.size(); ++j)
	
		{	
			y_coord = j/nbCells_;
			std::vector<std::size_t> indexes_for_cell (indexesForCellVertexes(j%nbCells_, y_coord, nbCells_ ));
			
			if (cells_[j] == Kind::water)
			{
	 
				 for (int i = 0; i <4 ; i++)
				 {
					 rockVertexes_[indexes_for_cell[i]].color.a = 0;
					 grassVertexes_[indexes_for_cell[i]].color.a = 0;
					 waterVertexes_[indexes_for_cell[i]].color.a = 255;
				 }
				 
				 
			}
			
			if (cells_[j] == Kind::rock)
			{	
				 
				 for (int i = 0; i <4 ; i++)
				 {
					 waterVertexes_[indexes_for_cell[i]].color.a = 0;
					 grassVertexes_[indexes_for_cell[i]].color.a = 0;
					 rockVertexes_[indexes_for_cell[i]].color.a = 255;
				 }
				 
			} 
			
			if (cells_[j] == Kind::grass)
			{
				
				 for (int i = 0; i <4 ; i++)
				 {
					 waterVertexes_[indexes_for_cell[i]].color.a = 0;
					 rockVertexes_[indexes_for_cell[i]].color.a = 0;
					 grassVertexes_[indexes_for_cell[i]].color.a = 255;
				 }
				 
			}
		}
	renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rseau);
	renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rsroche);
	renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rsherbe);
	
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
	nbCells_ = getTerrain()["cells"].toInt();
	cellSize_ = getTerrain()["size"].toDouble() / nbCells_;
	
	cells_ = std::vector<Kind> (nbCells_*nbCells_, Kind::rock);
}

void World::loadFromFile()
{
	reloadConfig();
	//getApp().getResPath(); // pourquoi on en a besoin?? je n'en ai pas beosin je crois ici.
	std::ifstream in;
	std::string i ("res/world.map");
	in.open(i);
	//getApp().getResPath()+getTerrain()["file"].toString() est "res/world.map"
	if (in.fail())
	{	
		throw std::runtime_error("AIEAIEAIEAIE");
	}
	else
	{		
		
			in >> nbCells_;
			in >> std::ws;
			in >> cellSize_;
			in >> std::ws;
			std::cout <<cells_.size() << std::endl;
			for (size_t i (0); i < cells_.size() ; ++i) // pas sur de taille
			{
			in >> std::ws;
			short var;
			Kind type;
			in >> var;
			type = static_cast<Kind>(var);
			cells_[i] = type;
			}
	}

	reloadCacheStructure();
	
	updateCache();
}

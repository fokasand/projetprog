#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include <iostream>
#include <fstream>
#include "Utility/Vertex.hpp"
#include <Random/Random.hpp>

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
	//nettoyage
	renderingCache_.clear();
	
	//dessine les textures dans le cache	
	colour("water",Kind::water,waterVertexes_);
	colour("rock",Kind::rock,rockVertexes_);
	colour("grass",Kind::grass,grassVertexes_);
	
	//affichage du cache
	renderingCache_.display();
}

//fonction colour en aide
void World::colour (std::string tex, Kind type, std::vector <sf::Vertex> vertex)
{
		sf::RenderStates rs;
		rs.texture = &getAppTexture(getTerrain()["textures"][tex].toString()); // ici pour la texture liée à la tex
		
		//coordonnée entière pour y
		int y_coord ;
		//parcour chaque cellule une à une 
		for(size_t j(0); j<cells_.size(); ++j)
		{	
			y_coord = j/nbCells_;
			//contient les coordonnées dans sommets
			std::vector<std::size_t> indexes_for_cell (indexesForCellVertexes(j%nbCells_, y_coord, nbCells_ ));
			
			if (cells_[j] == type)
			{
	 
				 for (int i = 0; i <4 ; i++)
				 {
					 //pour chaque sommet, colorie pour la texture appropriée (après les avoir toutes mises en transparence)
					 rockVertexes_[indexes_for_cell[i]].color.a = 0;
					 grassVertexes_[indexes_for_cell[i]].color.a = 0;
					 waterVertexes_[indexes_for_cell[i]].color.a = 0;
					 vertex[indexes_for_cell[i]].color.a = 255;
				 }
			}	
		}
	renderingCache_.draw(vertex.data(), vertex.size(), sf::Quads, rs);
}	


j::Value getTerrain()
{
	return getAppConfig()["simulation"]["world"];
}

void World::reset(bool regenerate)
{
		if (regenerate == 1)
		{
			//initialisation de seeds_ avec les graines
			for (int i(0); i < nbGrass_ ; ++i)
			{
				sf::Vertor2i coord (uniform(0, nbCells-1),uniform(0, nbCells-1));
				Seed graine(coord, Kind::grass);
				seeds_.push_back();
			}
			
			for (int i(0); i < nbWater_ ; ++i)
			{
				sf::Vertor2i coord (uniform(0, nbCells-1),uniform(0, nbCells-1));
				Seed graine(coord, Kind::water);
				seeds_.push_back();
			}
			
			for (size_t i(0); i <seeds_.size() ; ++i)
			{
				
			}
			
			
			// ajouter regeneration aléatoire du terrain
			//regenerate = 0;
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
	nbWater_ = getTerrain()["seeds"]["water"].toInt();
	nbGrass_ = getTerrain()["seeds"]["grass"].toInt();
	
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
			std::cout <<cells_.size() << std::endl; //test
			for (size_t i (0); i < cells_.size() ; ++i) // parcours le tableau de cellules
			{
				in >> std::ws;
				short var; //t'es sur que tu veux l'initialiser à chaque tour de boucle ?
				Kind type;
				in >> var;
				type = static_cast<Kind>(var);
				cells_[i] = type;
			}
	}

	reloadCacheStructure();
	updateCache();
}


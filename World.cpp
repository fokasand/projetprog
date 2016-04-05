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
	 
				 for (int i = 0; i <4 ; ++i)
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

void World::reset(bool regenerate=true)
{
	

	if (regenerate)		{
		reloadConfig();
		
		//initialisation de seeds_ avec les graines
		for (int i(0); i < nbGrass_ ; ++i)
		{
			sf::Vector2i coord (uniform(0, nbCells_-1) ,uniform(0, nbCells_-1));
			Seed graine = {coord,Kind::grass};
			seeds_.push_back(graine);
		}
		
		// d'abord grass puis water pour que l'eau ne soit pas écrasé si jamais il y avait 2 graines ayant les memes coordonnées
	
		for (int i(0); i < nbWater_ ; ++i)
		{
			sf::Vector2i coord (uniform(0, nbCells_-1),uniform(0, nbCells_-1));
			Seed graine = {coord,Kind::water};
			seeds_.push_back(graine);
		}
		
		for (size_t i(0); i <seeds_.size() ; ++i)
		{
			if (cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] != Kind::water)
			{
				cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] = seeds_[i].nature;
			}
		}

			
		reloadCacheStructure();
		updateCache();
	}
		
	else
		
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
	std::ifstream in;
	std::string i (getApp().getResPath()+getTerrain()["file"].toString());
	in.open(i);
	if (in.fail())
	{	
		throw std::runtime_error("AIEAIEAIEAIE");
	}
	else
	{		
		
		in >> nbCells_;
		in >> std::ws;
		in >> cellSize_;			in >> std::ws;
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

// mvt des graines
//un déplacement
void World::step()
{
	for (size_t i(0); i < seeds_.size(); ++i)
	{
		//déplacement aléatoire des graines d'herbe ou eau qui ne se teleportent pas 
		
		if (seeds_ [i].nature == Kind::grass or (seeds_[i].nature == Kind::water and !bernoulli(getTerrain()["seeds"]["water teleport probability"].toDouble())))
		{
			sf::Vector2i nouvelles (seeds_[i].coord.x+uniform(-1,1),seeds_[i].coord.y+uniform(-1,1)); //truc intermediaire à enlever
			
			//remettre la graine dans la fenetre si elle depasse
			debVect(nouvelles);
			seeds_[i].coord = nouvelles;
		} else {
			std::cerr << "cas 2:  eau tele" << std::endl;
			//la graine d'eau se teleporte 
			sf::Vector2i nouvelles (uniform(0, nbCells_-1),uniform(0, nbCells_-1)); //à modulariser !! 
			seeds_[i].coord = nouvelles ;
		}
		
		//meme code que dans reset A MODULARISER permet de donner le kind de la graine à la cellule de memes coordonnées 
			if (cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] != Kind::water)
			{
				cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] = seeds_[i].nature;
			}
	}
}

// i déplacements			
void World::steps( int i, bool regeneration = false)
{
	for (int j(0); j < i; ++j)
	{
		step();
	}
	if (regeneration)
	{
		updateCache();
	}
}

//MODULARISER LE PLURIEL : fonction repeat pour smooths et steps

//smooth
void World::smooth()
{	auto copie_de_cells_ = cells_;
	
	for (int x(0); x < nbCells_; ++x)
	{
		for (int y(0); y<nbCells_; ++y)
		{
			int Someau(0);
			int Somherbe(0);
			int Somtot(0);
			
			for (int h(-1); h<1; ++h)
			{
				for (int v(-1); v<1; ++v)
				{			
					if (not((v==0) and (h==0)))
					{
						if ((x+h>=0) and(y+v>=0) and (y+v <=nbCells_-1) and ( x+h<= nbCells_-1))
						{
							if (cells_[toUnid(x+h,y+v)]== Kind::water)
							{
								++Someau;
								++Somtot;
							}
							if (cells_[toUnid(x+h,y+v)]== Kind::grass)
							{
								++Somherbe;
								++Somtot;
							}
							else 
							{
								++Somtot;
							}
						}
					}				
				}
			}
			double n(Somherbe/Somtot);
			if (n <= getTerrain()["generation"]["smoothness"]["grass neighbourhood ratio"].toDouble())
			{
				cells_[toUnid(x,y)] = Kind::grass;
			}
			n=Someau/Somtot;
			if (n <= getTerrain()["generation"]["smoothness"]["water neighbourhood ratio"].toDouble())
			{
				cells_[toUnid(x,y)] = Kind::water;
			}
		}
	std::swap(cells_, copie_de_cells_);	
	}
}

void World::smooths( int i, bool regeneration = false)
{
	for (int j(0); j < i; ++j)
	{
		smooth();
	}
	if (regeneration)
	{
		updateCache();
	}
}

//fonctions conversions:
int World::toUnid (int x, int y)
{
	return (y-1)*nbCells_+x;
}

sf::Vector2i World::toBid( int x)
{
	sf::Vector2i retour;
	retour.x= x%nbCells_;
	retour.y= x/nbCells_;
	return retour;
}

//débordements de la fenetre d'affichage

int World::debSup (int c)
{
	if (c> nbCells_)
	{
		c =nbCells_-1;
	}
	return c;
}

int World::debInf (int c)
{
	if (c < 0)
	{
		c =0;
	}
	return c;
}

void World::debVect (sf::Vector2i coord)
{
	debSup(coord.x);
	debInf(coord.x);
	
	debSup(coord.y);
	debInf(coord.y);
	
}


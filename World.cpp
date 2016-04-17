#include <Application.hpp>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include <iostream>
#include <fstream>
#include "Utility/Vertex.hpp"
#include <Random/Random.hpp>
#include <cmath>

//initialisation ensemble de textures
void World::reloadCacheStructure()

{	//vector contenant l'ensemble de sommets
	std::vector<sf::Vertex> Vertexes (generateVertexes(getTerrain()["textures"],nbCells_ ,cellSize_));
	
	//initialisation à tous les sommets de la grille
	grassVertexes_=Vertexes;
	waterVertexes_=Vertexes;
	rockVertexes_=Vertexes;
	humidityVertexes_ =Vertexes;
	
	//initialisation de la texture
	renderingCache_.create(nbCells_*cellSize_, nbCells_*cellSize_);
	humideCache_.create(nbCells_*cellSize_, nbCells_*cellSize_);
}

//fonction draw
void World::drawOn(sf::RenderTarget& target)
{
	if (getTerrain()["show humidity"].toBool())
	{	
		sf::RenderStates rshumide;
		humideCache_.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads, rshumide);
		sf::Sprite cache(humideCache_.getTexture());
		target.draw(cache);
	}
	else
	{
	sf::Sprite cache(renderingCache_.getTexture());
	target.draw(cache);
	}
}

//mettre a jour rendering_Cache
void World::updateCache()
{
	//nettoyage
	renderingCache_.clear();
	humideCache_.clear();
	
	//calcule les niveaux d'humidité max et min
	minmaxhumid();
	//dessine les textures dans le cache	
	
	colour("rock",Kind::rock,rockVertexes_);
	colour("grass",Kind::grass,grassVertexes_);
	colour("water",Kind::water,waterVertexes_);
	
	//affichage du cache
	renderingCache_.display();
	humideCache_.display();
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
			
			// virer y coord, remplacer ligne d'après par:
			
			//std::vector<std::size_t> indexes_for_cell (indexesForCellVertexes(toBid(j).x, toBid(j).y, nbCells_ ));
			
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
			
			if (type == Kind::water) // on ne remplit le cache d'humidité seulement lorsqu'on remplit les cellules d'eau, on choisit cela arbitrairement pour ne remplir les vertexes
												// qu'une fois.
					 {
						unsigned int niveaubleu (((humide_[j] - minHumidity)/ (maxHumidity - minHumidity)) * 255);
						//std::cout << niveaubleu << std::endl;
						for (int i(0); i<4; ++i)
						{
							humidityVertexes_[indexes_for_cell[i]].color = sf::Color(0, 0, niveaubleu);
						}
					 } 
					 	
		}
		//std::cout << minHumidity << " " << maxHumidity << std::endl;
	renderingCache_.draw(vertex.data(), vertex.size(), sf::Quads, rs);
}	

//raccourci 
j::Value getTerrain()
{
	return getAppConfig()["simulation"]["world"];
}

void World::reset(bool regenerate=true)
{
	

	if (regenerate)		{
		reloadConfig();
		reloadCacheStructure();
		seeds_.clear();
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
		
			humidcalc(toUnid(coord.x,coord.y)); // fais pour 1 graine.
		}
		
		for (size_t i(0); i <seeds_.size() ; ++i)
		{
			seedTocell(i);
		}

		steps(getTerrain()["generation"]["steps"].toInt(),false); //false est par défaut normalement, pk ne marche pas ?
		std::cout << "oula" << std::endl;
		smooths(getTerrain()["generation"]["smoothness"]["level"].toInt(),false);	
		
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
	
	humidityRange_=0;
	e = getTerrain()["generation"]["humidity"]["initial level"].toDouble();
	l = getTerrain()["generation"]["humidity"]["decay rate"].toDouble();
	double t(getTerrain()["generation"]["humidity"]["threshold"].toDouble());

	while (e * exp(-humidityRange_ / l) > t)
	{
		 ++humidityRange_;
	}
	cells_ = std::vector<Kind> (nbCells_*nbCells_, Kind::rock);
	humide_ = std::vector<double>(nbCells_*nbCells_, 0.0);
}

//humidité
void World::humidcalc(int pos)
{
	int xpos(toBid(pos).x);
	int ypos(toBid(pos).y);
	std::cout << "x=" << xpos << " y=" << ypos << " pos=" << pos << std::endl;
	for (int x(-humidityRange_);x<=(humidityRange_+ 1); ++x)
	{
		for (int y(-humidityRange_);y<=(humidityRange_+ 1); ++y)
		{
			if (std::hypot(xpos+x, ypos+y) <= humidityRange_)
			{
				if ((xpos+x >= 0) and (ypos+y >= 0) and (xpos+x <nbCells_) and (ypos+y < nbCells_))
				//modulariser cette partie dans un booléen que l'on utilisera aussi pour smooth?
				{
				humide_[toUnid(xpos+x,ypos+y)] += e * exp(- std::hypot(xpos+x, ypos+y)  / l);
				}
			}
		}
	}
}

void World::minmaxhumid()
{
minHumidity=100000;
maxHumidity=0;

for (size_t j(0); j<humide_.size(); ++j)
{
	if ( humide_[j] > maxHumidity)
	{
		maxHumidity = humide_[j];
	}
	if ( humide_[j] < minHumidity)
	{
		minHumidity = humide_[j];
	}
}
}

// load la configuration de la carte depuis un fichier
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
		in >> cellSize_;			
		in >> std::ws;
		for (size_t i (0); i < cells_.size() ; ++i) 
		{
			
			in >> std::ws;
			short var;
			Kind type;
			in >> var;
			type = static_cast<Kind>(var);
			cells_[i] = type;
		}
		in >> std::ws; // pour sauter le retour a la ligne
		for (size_t i (0); i < humide_.size() ; ++i) 
		{
			
			in >> std::ws;
			double var;
			in >> var;
			humide_[i] = var;
		}
	}
	reloadCacheStructure();
	updateCache();
}

//sauvegarde la configuration dans un fichier
void World::saveToFile()
{		
	std::ofstream out;
	std::string i (getApp().getResPath()+getTerrain()["file"].toString());
	out.open(i);
	if (out.fail())
	{	
		throw std::runtime_error("fichier non ouvert");
	}
	else
	{		
		
		out << nbCells_ << std::endl;
		out << cellSize_ << std::endl;
		for (size_t i (0); i < cells_.size() ; ++i) 
		{
			short var (static_cast<short>(cells_[i]));
			out << var << " ";
		}
		out << std::endl;
		for (size_t i (0); i < humide_.size() ; ++i) 
		{
			double var(humide_[i]);
			out << var << " ";
		}
	}

} 

// mvt des graines
//un déplacement
void World::step()
{
	double waterprob (getTerrain()["seeds"]["water teleport probability"].toDouble());
	for (size_t i(0); i < seeds_.size(); ++i)
	{
		//déplacement aléatoire des graines d'herbe ou eau qui ne se teleportent pas 
		
		if (seeds_[i].nature == Kind::grass or (seeds_[i].nature == Kind::water and !bernoulli(waterprob)))
		{
			sf::Vector2i nouvelles (seeds_[i].coord.x+uniform(-1,1),seeds_[i].coord.y+uniform(-1,1)); //truc intermediaire à enlever
			
			//remettre la graine dans la fenetre si elle depasse
			debVect(nouvelles);
			seeds_[i].coord = nouvelles;
		} else {
			//la graine d'eau se teleporte 
			sf::Vector2i nouvelles (uniform(0, nbCells_-1),uniform(0, nbCells_-1)); //à modulariser !! 
			seeds_[i].coord = nouvelles ;
		}
		
		seedTocell(i);
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

//MODULARISER LE PLURIEL : fonction repeat pour smooths et steps

//smooth
void World::smooth()
{	auto copie_de_cells_ = cells_;
	
	double wsmooth (getTerrain()["generation"]["smoothness"]["water neighbourhood ratio"].toDouble());
	double gsmooth (getTerrain()["generation"]["smoothness"]["grass neighbourhood ratio"].toDouble());
	for (int x(0); x < nbCells_; ++x)
	{
		for (int y(0); y < nbCells_; ++y)
		{
			double Someau(0.0);
			double Somherbe(0.0);
			double Somtot(0.0);
			for (int h(-1); h<=1; ++h)
			{
				for (int v(-1); v<=1; ++v)
				{		
					if (not((v==0) and (h==0)))
					{
						if ((x+h>=0) and (y+v>=0) and (y+v <=nbCells_-1) and (x+h<= nbCells_-1))
						{
						
							if (copie_de_cells_[toUnid(x+h,y+v)] == Kind::water)
							{
								++Someau;
							}
							if (copie_de_cells_[toUnid(x+h,y+v)] == Kind::grass)
							{
								++Somherbe;
							}
							++Somtot;
						}
					}				
				}
			}
			
			if ((Someau/Somtot) >= wsmooth )
			{
				if (copie_de_cells_[toUnid(x,y)] != Kind::water)
				{
				copie_de_cells_[toUnid(x,y)] = Kind::water;
				humidcalc(toUnid(x,y));
				}
			}
			if ((Somherbe/Somtot) >= gsmooth)
			{	
				if (copie_de_cells_[toUnid(x,y)] == Kind::rock)
				{
				copie_de_cells_[toUnid(x,y)] = Kind::grass;
				}
			}
		}
	}
std::swap(cells_, copie_de_cells_);	
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
	if((y)*nbCells_+x <0)
	{
		throw std::out_of_range( "les coordonnées reçues inférieures à l'indice minimum possible" ); 
	}
	if((y)*nbCells_+x > nbCells_*nbCells_)
	{
		throw std::out_of_range( "les coordonnées reçues dépassent l'indice maximum possible" ); 
	}
	
	
	return y*nbCells_+x;
}

sf::Vector2i World::toBid( int x)
{
	sf::Vector2i retour;
	retour.x= x%nbCells_;
	retour.y= x/nbCells_;
	return retour;
}

//débordements de la fenetre d'affichage

void World::debSup (int& c)
{
	if (c>= nbCells_)
	{
		c =nbCells_-1;
	}
}

void World::debInf (int& c)
{
	if (c < 0)
	{
		c =0;
	}
}

void World::debVect (sf::Vector2i& coord)
{
	debSup(coord.x);
	debInf(coord.x);
	
	debSup(coord.y);
	debInf(coord.y);
	
}

//donner le type de la graine à la cellule ayant les mêmes coordonées 
void World::seedTocell(size_t i)
{
	if (cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] != Kind::water)
			{
				cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] = seeds_[i].nature;
				if (seeds_[i].nature == Kind::water)
				{
					humidcalc(toUnid(seeds_[i].coord.x, seeds_[i].coord.y));
				}
			}
}

#ifndef WORLD_H
#define WORLD_H
#include "Utility/Vertex.hpp"
#include "Utility/Utility.hpp"
#include <Random/Random.hpp>
#include <SFML/Graphics.hpp>

j::Value getTerrain();

enum class Kind : short { grass, water, rock};
struct Seed {
	sf::Vector2i coord;
Kind nature;	
};

class World{
private :
	//variable regroupant la texture totale à afficher
	sf::RenderTexture renderingCache_;
			
	//sommets liés à chaque texture
	std::vector <sf::Vertex> grassVertexes_, 
							waterVertexes_,
							rockVertexes_;
	//humidité:
	std::vector<double> humide_;
	int humidityRange_;
	double e;
	double l;
	std::vector <sf::Vertex> humidityVertexes_;
	double minHumidity;
	double maxHumidity;
	//cache de l'humidité:
	sf::RenderTexture humideCache_;
	
	//tableau de cellules					 
	std::vector<Kind> cells_;
	int nbCells_;
	float cellSize_;
	
	//graines
	std::vector<Seed> seeds_;		
	int nbWater_;
	int nbGrass_;
	
	//débordements de la fenetre d'affichage
	
	//cordonnée superieure a nbCells
	void debSup (int& c);
	
	//coordonnée inferieure à nbCells
	void debInf(int& c);
	
	//verifie le vecteur de coordonnées
	void debVect (sf::Vector2i& coord); 
	
	//mvt graines
	void step();
	
	//lissage
	void smooth();
	
	//nettoie renderingCache, y dessine les 3 Vertexes en couleur ou transparence selon le type de la cellule, affiche la cache
	void updateCache();
	
	//creation des trois Vertexe liés au texture et création du cache contenant les textures à afficher
	void reloadCacheStructure();
	
	//donner le type de la graine à la cellule ayant les mêmes coordonées 
	void seedTocell(size_t i);
	
public:
	//calcule le taux d'humidité d'une cellule et le range dans humide
	void humidcalc(int pos);
	
	//calcule les minimum et maximum d'humiditö et les stocke en attribut pour les utiliser dans updatecache
	void minmaxhumid();
	
	//charge les paramètres  de la fenetre à partir du fichier
	void reloadConfig();

	//dessine la texture stockée dans renderingCache sur la fenetre
	void drawOn(sf::RenderTarget& target);
	
	//reset
	void reset(bool regenerate);
	
	//load toute una map depuis un fichier
	void loadFromFile();
	
	//sauvegarde la configuration dans un fichier
	void saveToFile();
	
	//mvt graines
	void steps( int i, bool regeneration);
	
	//lissage des textures
	void smooths(int i, bool regeneration);
	
	//conversions
	int toUnid (int x, int y); // to unidimensional 
	sf::Vector2i toBid( int x); // to bidimensional
	
	//getter utilisé dans Env:
	float getcellSize_();
	int getnbCells_();
	std::vector<Kind> getcells ();
	
	//verifie que une fleur peut être plantée
	bool isGrowable(const Vec2d&p);
	
	//convertir de graphique à tableau
	int toGrid(double p);

	};

#endif

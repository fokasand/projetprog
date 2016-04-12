#ifndef WORLD_H
#define WORLD_H


enum class Kind : short { grass, water, rock};
struct Seed {
	sf::Vector2i coord;
	Kind nature;	
};

class World{
	private :
			// destructeur
			
			//variable regroupant la texture totale à afficher
			sf::RenderTexture renderingCache_;
			
			//sommets liés à chaque texture
			 std::vector <sf::Vertex> grassVertexes_, 
								waterVertexes_,
								 rockVertexes_;
			
			//tableau de cellules					 
			std::vector<Kind> cells_;
			int nbCells_;
			float cellSize_;
			
			//graines
			std::vector<Seed> seeds_;		
			int nbWater_;
			int nbGrass_;
			
			//colore ou met en transparence les sommets dans chache vertex et 
			void colour (std::string tex, Kind type, std::vector <sf::Vertex> vertex);
			
			//conversions
			int toUnid (int x, int y); // to unidimensional 
			sf::Vector2i toBid( int x); // to bidimensional
			
			//débordements de la fenetre d'affichage
			
			//cordonnée superieure a nbCells
			void debSup (int& c);
			//coordonnée inferieur à nbCells
			void debInf(int& c);
			//verifie le vecteur de coordonnées
			void debVect (sf::Vector2i& coord); 
			
			//mvt graines
			void step();
			
			//lissage
			void smooth();
			
			//creation des trois Vertexe liés au texture et création du cache contenant les textures à afficher
			void reloadCacheStructure();
			
			//donner le type de la graine à la cellule ayant les mêmes coordonées 
			void seedTocell(size_t i);
			
			//bloquer la copie
			World& operator=(World const&) = delete;
			
	public:
			
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
			
			//lissage
			void smooths(int i, bool regeneration);
			
			//nettoie renderingCache, y dessine les 3 Vertexes en couleur ou transparence selon le type de la cellule, affiche la cache
			void updateCache();
		};
	
j::Value getTerrain();


#endif

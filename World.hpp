#ifndef WORLD_H
#define WORLD_H


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
			
			//tableau de cellules					 
			std::vector<Kind> cells_;
			int nbCells_;
			float cellSize_;
			
			//graines
			std::vector<Seed> seeds_;		
			int nbWater_;
			int nbGrass_;
			
			//coloriage et transparence
			void colour (std::string tex, Kind type, std::vector <sf::Vertex> vertex);
	public:
			//mettre a jour rendering_Cache
			void updateCache();
			
			//initialisation ensembles de textures
			void reloadCacheStructure();

			//draw
			void drawOn(sf::RenderTarget& target);
			
			//reload
			void reloadConfig();
			
			//reset
			void reset(bool regenerate=false);
			
			//fichier
			void loadFromFile();
			
			
	};
	
j::Value getTerrain();


#endif

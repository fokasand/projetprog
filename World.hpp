#ifndef WORLD_H
#define WORLD_H

class World{
	private :
	//variable regroupant la texture totale à afficher
			 sf::RenderTexture renderingCache_;
	//sommets liés à chaque texture
			 vector <sf::Vertex> grassVertexes_, 
				   		waterVertexes_,
						rockVertexes_;
	public:
	
	};

//initialisation ensembles de textures
void reloadCacheTructure();

//draw
void drawOn(sf::RenderTarget& target);

#endif


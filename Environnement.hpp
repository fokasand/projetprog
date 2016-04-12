#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

class Environnement {
	
	private:
	World World_;
	//regeneration de l'environnement
	void reset();
	//evolution de l'environnement sur un temps dt
	void update(sf::Time dt);
	
	//dessin de l'environnement
	void drawOn(sf::RenderTarget& target);
};

#endif

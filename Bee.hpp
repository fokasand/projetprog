#ifndef BEE_H
#define BEE_H
#include "Collider.hpp"
#include "Hive.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include "CFSM.hpp"

enum MoveMode {Rest,Random,Targeted};

class Bee: public Collider, public Drawable, public Updatable, 
		public CFSM 
{
public:	
	//constructeur
	Bee(Vec2d centre,
		double rayon,
		 Hive* hive,
		 double energy,
		 double amplitude,
		 sf::Texture texture);

	//morte si energie nulle
	bool isDead();

	//déplacement : calcule nouvelles positions et vitesse
	void update(sf::Time dt);
	
	void drawOn(sf::RenderTarget& targetWindow) const;
	
	//méthodes de mouvement
	//aléatoire
	void randomMove(sf::Time dt);
	//non aléatoire
	void move(sf::Time dt);
	//ciblé
	void targetMove(sf::Time dt);
	
	//permet d'ulitiser le polymophisme de getConfig
	void setloss();
	
	//retourne le mode de déplacement 
	MoveMode getMoveMode() const;
	
	// retourne le jvalue utilisé pour raccourcir
	virtual j::Value const& getConfig() const = 0;
	j::Value const& getBeeConfig() const; // utilisé pour le constructeur
	
protected:
	Hive* hive_;
	double energy_;
	Vec2d speed_;
	double prob;
	double alpha_max;
	sf::Texture texture_;
	static const State IN_HIVE;
	//tableau rassemblant les états communs à touts types d'abeilles	
	static std::vector<State> etats_ ;
	//attribut désigneant le mode de déplacement
	MoveMode moveMode_;
	
	//valeurs initilialisées dans les constructeurs de Worker et Scout
	double restloss_;
	double moveloss_;

};	
#endif

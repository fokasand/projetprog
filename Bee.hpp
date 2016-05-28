#ifndef BEE_H
#define BEE_H
#include "Collider.hpp"
#include "Hive.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include "CFSM.hpp"

class WorkerBee;
class ScoutBee;

enum MoveMode {Rest,Random,Targeted};

class Bee: public Collider, public Drawable, public Updatable, 
		public CFSM 
{
public:	
	//constructeur
	Bee(vector<State> states,
		Vec2d centre,
		double rayon,
		Hive* hive,
		double energy,
		double amplitude,
		sf::Texture texture);
		 
	// destructeur
	
	virtual ~Bee();
		 	 
	//morte si energie nulle
	bool isDead() const;

	//déplacement : calcule nouvelles positions et vitesse
	void update(sf::Time dt);
	
	void drawOn(sf::RenderTarget& targetWindow) const;
	
	//méthodes de mouvement
	//aléatoire
	void randomMove(sf::Time dt);
	//non aléatoire
	void move(sf::Time dt);
	//ciblé
	void targetMove(sf::Time dt, Vec2d target);
	
	//permet d'ulitiser le polymophisme de getConfig
	void setTout();
	
	//retourne le mode de déplacement 
	MoveMode getMoveMode() const;
	
	// retourne le jvalue utilisé pour raccourcir
	virtual j::Value const& getConfig() const = 0;
	j::Value const& getBeeConfig() const; // utilisé pour le constructeur
	
	// essaye de bouger l'abeille selon son vecteur vitesse, si c est impossible, fais tourner la direction.
	bool movebee(sf::Time dt);
	
	//manger la nectar
	void eat(sf::Time dt);
	
	//passer une adresse à la mémoire
	void setMemory(Vec2d* address);
	
	//rend la position de la fleur visible
	Vec2d* visibleFlower();
	
	virtual void interact(Bee* other) = 0;
	virtual void interactWith(ScoutBee* scouting) = 0;
	virtual void interactWith(WorkerBee* working) = 0;
	
	bool isBeeInHive();
	
	Vec2d* getMemory() const;
	
private:
	void changeEnergy(double quant);
	
protected:
	Hive* hive_;
	double energy_;
	Vec2d speed_;
	sf::Texture texture_;
	double prob;
	double alpha_max;
	//attribut désigneant le mode de déplacement
	MoveMode moveMode_;
	Vec2d* memory_;
	// temps pendant lequel l'abeille est déviée
	sf::Time avoidanceClock_;
	
	//valeurs initilialisées dans les constructeurs de Worker et Scout
	double restloss_;
	double moveloss_;
	Vec2d target_;
	
	//voir si ne peuvent être mis dans Bee avec un setter polymorphique
	double cons_rate;
	double enmin_hive;
	double visibility_;
	
	string statestring_;
	
	//permet de repertorier les abeilles dans une ruche
	bool isInHive_;
	
private:
	double delay;
};	
#endif

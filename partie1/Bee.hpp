#ifndef BEE_H
#define BEE_H
#include "Collider.hpp"
#include "Hive.hpp"
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include "CFSM.hpp"

//predéfinition des classes pour eviter la dépendance circulaire
class WorkerBee;
class ScoutBee;

//definit les valeurs possibles pour le type de mouvement: Repos, mouvement Aléatoire ou mouvement Ciblé
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
	
	//dessin des abeilles
	void drawOn(sf::RenderTarget& targetWindow) const;
	
	//méthodes de mouvement
	//aléatoire
	void randomMove(sf::Time dt);
	//ciblé
	void targetMove(sf::Time dt, Vec2d target);
	//méthode appelant randomMove ou targetMove selon le mode de mouvement courant
	void move(sf::Time dt);
	
	//permet d'ulitiser le polymophisme de getConfig pour initialiser des attributs
	void setTout();
	
	//retourne le mode de déplacement 
	MoveMode getMoveMode() const;
	
	// retourne le jvalue utilisé pour raccourcir
	virtual j::Value const& getConfig() const = 0;
	j::Value const& getBeeConfig() const; // utilisé pour le constructeur
	
	// essaye de bouger l'abeille selon son vecteur vitesse, si c est impossible, fais tourner la direction.
	bool movebee(sf::Time dt);
	
	//manger du nectar
	void eat(sf::Time dt);
	
	//passer une adresse à la mémoire
	void setMemory(Vec2d* address);
	
	//rend la position de la première fleur visible
	Vec2d* visibleFlower();
	
	//méthode d'interaction non définis pour une abeille générique
	virtual void interact(Bee* other) = 0;
	virtual void interactWith(ScoutBee* scouting) = 0;
	virtual void interactWith(WorkerBee* working) = 0;
	
	//indique si l'abeille se trouve dans la ruche ou non 
	bool isBeeInHive();
	
	//rend la position en mémoire de l'abeille
	Vec2d* getMemory() const;
	
private:
	void changeEnergy(double quant);
	double delay;
	
protected:
	//pointeur sur sa ruche
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
	double cons_rate;
	double enmin_hive;
	double visibility_;
	
	//permet d'afficher l'état dans lequel se trouve l'abeille
	string statestring_;
	
	//permet de repertorier les abeilles dans une ruche
	bool isInHive_;

};	
#endif

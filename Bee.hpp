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
		 
	// destructeur
	
	virtual ~Bee() = default ;
		 	 

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
	void setLoss();
	
	//retourne le mode de déplacement 
	MoveMode getMoveMode() const;
	
	// retourne le jvalue utilisé pour raccourcir
	virtual j::Value const& getConfig() const = 0;
	j::Value const& getBeeConfig() const; // utilisé pour le constructeur
	
	// essaye de bouger l'abeille selon son vecteur vitesse, si c est impossible, fais tourner la direction.
	bool movebee(sf::Time dt);
	
protected:
	Hive* hive_;
	double energy_;
	Vec2d speed_;
	double prob;
	double alpha_max;
	sf::Texture texture_;
	static const State IN_HIVE;
	static const State TO_HIVE;
	Vec2d* memory_;
	
	//tableau rassemblant les états communs à touts types d'abeilles	
	static std::vector<State> etats_ ;
	
	// temps auquel l'horloge est initialisé si l'abeille doit éviter un obstacle
	static sf::Time const delay_;
	
	//attribut désigneant le mode de déplacement
	MoveMode moveMode_;
	
	//valeurs initilialisées dans les constructeurs de Worker et Scout
	double restloss_;
	double moveloss_;
	
	Vec2d target_;
	
	// temps pendant lequel l'abeille est déviée
	sf::Time avoidanceClock_;
	
	//voir si ne peuvent être mis dans Bee avec un setter polymorphique
	static const double cons_rate;
	static const double enmin_hive;
};	
#endif

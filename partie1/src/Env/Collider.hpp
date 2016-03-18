
#include <cassert>
#ifndef COLLIDER_H
#define COLLIDER_H
#include "Utility/Vec2d.hpp"

using namespace std;

class Collider 
{
	private :
			Vec2d centre;
			double rayon;
	public :
	
	//constructeur, verifier que r positif ou nul,
	Collider (Vec2d c, double r);
	Collider (const Collider& a);
			
	//getters
	const Vec2d& getPosition() const;
	double getRadius() const;
			
	//déplacement
	Vec2d directionTo (Collider c) const;
	
	// distances
	Vec2d directionTo (Vec2d to) const;
	double distanceTo(Vec2d to) const;
	
	//clamping
	void clamping(Vec2d& c);
	
	// =
	Collider operator= ( Collider b);
	
	// booléens
	
	bool isColliderInside(const Collider& other) const;
	bool isColliding(const Collider& other) const;
	bool isPointInside(const Vec2d& point) const;
	
	//opérateurs
	bool operator>(const Vec2d& point);
	bool operator>(const Collider& body2);
	bool operator|(const Collider& body2);
	
	//aide pour <<
	
	std::ostream& affiche(std::ostream& sortie) const;
	
	
	
			
};


#endif

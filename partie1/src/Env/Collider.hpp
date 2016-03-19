
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
	Collider (const Vec2d& c,const double& r);
	Collider (const Collider& a);
			
	//getters
	const Vec2d& getPosition() const;
	double getRadius() const;
			
	//déplacement
	Vec2d directionTo (const Collider& c) const;
	Vec2d directionTo (Vec2d to) const;
	void  move(Vec2d dx);
	
	
	// distances
	
	double distanceTo(const Vec2d& to) const;
	
	//clamping
	void clamping();
	
	// =
	Collider& operator=(Collider b);
	
	// booléens
	
	bool isColliderInside(const Collider& other) const;
	bool isColliding(const Collider& other) const;
	bool isPointInside(const Vec2d& point) const;
	
	//opérateurs
	
	bool operator>(const Vec2d& point) const;
	bool operator>(const Collider& body2) const;
	bool operator|(const Collider& body2) const;
	Collider& operator+=(const Collider& b); // I4m not sure if return type should be Collider or Collider&
	
	//aide pour <<
	
	std::ostream& affiche(std::ostream& sortie) const;

};

std::ostream& operator<<(std::ostream& sortie, const Collider& c);



#endif

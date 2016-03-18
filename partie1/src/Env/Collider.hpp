
#include <cassert>
#ifndef COLLIDER_H
#define COLLIDER_H
#include "Utility/Vec2d.hpp"

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
	Vec2d directionTo (Vec2d to) const;
	Vec2d Collider::directionTo (Collider c) const
	//clamping
	void clamping(Vec2d& c);
	
	// =
	Collider operator= ( Collider b);
	
	// booléens
	
	bool isColliderInside(const Collider& other) const;
	bool isColliding(const Collider& other) const;
	bool isPointInside(const Vec2d& point) const;
	
	// distances
	Vec2d directionTo (Vec2d to) const;
	double distanceTo(Vec2d to) const;
	
			
};


#endif

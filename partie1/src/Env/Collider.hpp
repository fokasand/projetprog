
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
			
			
	//getters
			const Vec2d& getPosition ();
			double getRadius();
			
};


#endif

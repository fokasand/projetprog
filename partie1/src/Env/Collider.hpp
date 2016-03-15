
#include <cassert>
#ifndef COLLIDER_H
#define COLLIDER_H
#include <../Utility/Vec2d.hpp>

class Collider 
{
	private :
			Vec2d centre;
			double rayon;
	public :
	//constructeur, verifier que r positif ou nul,
			Collider (Vec2d c, double r) :
			centre(c),
			rayon(r)
			{
				assert(rayon >=0);
			}
			
	//getters
			const Vec2d& position ()
			{
				return centre;
			}
			
			double ()
			{
				return rayon;
			}
}


#endif

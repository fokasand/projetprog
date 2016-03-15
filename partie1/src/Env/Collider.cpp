#include "Collider.h"
#include <iostream>
#include <Application.hpp>

//constructeur, verifier que r positif ou nul
Collider::Collider (Vec2d c, double r) :	
		centre(c),
		rayon(r)
{
	assert(rayon >=0);
}

void clamping(vec2d& c)
{
	while (c.x> worldsize.x)
	{
		c.x-= worldsize.x;
	}
	
	while (c.y > worldsize.y)
	{
		c.y-=worldsize.y;
	}
}

// getters

const Vec2d&Collider::getPosition ();
{
	return centre;
}

double getRadius()
{
	return rayon;
}

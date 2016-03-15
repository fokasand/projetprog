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

// getters

const Vec2d&Collider::getPosition ();
{
	return centre;
}

double getRadius()
{
	return rayon;
}

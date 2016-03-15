#include "Collider.hpp"
#include <iostream>
#include <Application.hpp>

//constructeur, verifier que r positif ou nul
Collider::Collider (Vec2d c, double r) :	
		centre(c),
		rayon(r)
{
	assert(rayon >=0);
}

Collider::Collider (const Collider& a) :
	centre( a.getPosition()),
	rayon(a.getRadius())
	{}
	
//clamping

void Collider::clamping(Vec2d& c)
{
	Vec2d worldSize = getApp().getWorldSize();
	double width = worldSize.x;
	double height= worldSize.y;
	
	while (c.x> width)
	{
		c.x -= worldSize.x;
	}
	
	while (c.y> height)
	{
		c.y -= worldSize.y;
	}
}

/*void Collider::clamping(Vec2d& c)
{
	auto worldSize = getApp().getWorldSize();
	auto width= worldSize.x;
	auto height = worldSize.y;
	
	while (c.x> width)
	{
		c.x-= width;
	}
	
	while (c.y > height)
	{
		c.y-=height;
	}
} */

// getters

 const Vec2d& Collider::getPosition ()
{
	return centre;
}

double Collider::getRadius()
{
	return rayon;
}

//deplacement

void Collider::directionTo (Vec2d to)
{
	Vec2d from (centre);
	
}


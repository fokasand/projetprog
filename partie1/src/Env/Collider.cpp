#include "Collider.hpp"
#include <iostream>
#include <Application.hpp>
#include <vector>

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

Vec2d Collider::directionTo (Vec2d to)
{
	Vec2d from (centre);
	auto worldSize = getApp().getWorldSize();
	double* dimension(nullptr);
	double* coordinatei(nullptr);
	double* coordinatef(nullptr);
	//creer un pointeur
	
	for (int i(0); i <2 ; i++)
	{
		if (i==0)
		{	//*dimension= new double(worldSize.x); Set for x
			dimension = new double(worldSize.x);
			//*dimension;
			coordinatei= new double(from.x);
			coordinatef= new double(to.x);
			
		}
		
		if (i==1)
		{	// Set for y
			*dimension = worldSize.y;
			*coordinatei = from.y;
			*coordinatef = to.y;	
		}
		
		if ((*coordinatei-*coordinatef)<(-*dimension/2))
		{
			*coordinatef-=*dimension;
		}
		if ((*coordinatei-*coordinatef)>(*dimension/2))
		{
			*coordinatef+=*dimension;
		}
	}
	
	Vec2d result ((from.x-to.x),(from.y-to.y));
	
	return result;
}

double distanceTo(Vec2d to)
{
	return directionTo(to).lenght();
}

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
	Vec2d vect(to);
	
	auto worldSize = getApp().getWorldSize();
	auto width= worldSize.x;
	auto height = worldSize.y;
	
	//creer un vector contenant toutes les possibilités de Vec2d
	vector <Vec2d> tab_test(8);
	tab_test.pushback(0,height);
	tab_test.pushback(0,-height);
	tab_test.pushback(width,0);
	tab_test.pushback(-width,0);
	tab_test.pushback(width,height);
	tab_test.pushback(width,-height);
	tab_test.pushback(-width,-height);
	tab_test.pushback(-width,height);
	
	//tester les vecteurs possibles et rendre le plus court
	for (int i(0); i <8; i++)
	{
		if (distance(from,tab_test[i]) < distance (from,vect)//test, si distanceplus petite
		{	vect = tab_test[i];								//nouvelle valeur pour vect
		}
	}
	return vect;
}


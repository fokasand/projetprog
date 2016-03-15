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

Collider::Collider (const Collider& a)
{
	centre = a.getPosition();
	rayon = a.getRadius();
}
	
Collider Collider:: operator= ( Collider b)
{
	swap (*this, b);
	return *this;
}
	
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
	vector <Vec2d> tab_test(8);	//ne reconnait pas le type vector,c'est un problème
	tab_test.push_back(to.x,to.y+height);
	tab_test.push_back(to.x,to.y-height);
	tab_test.push_back(to.x+width,to.y);
	tab_test.push_back(to.x-width,to.y);
	tab_test.push_back(to.x+width,to.y+height);
	tab_test.push_back(to.x+width,to.y-height);
	tab_test.push_back(to.x-width,to.y-height);
	tab_test.push_back(to.x-width,to.y+height);
	
	//tester les vecteurs possibles et rendre le plus court
	for (int i(0); i <8; i++)
	{
			if (sqrt(from.dot(tab_test[i])) < sqrt(from.dot(vect)))//test, si distanceplus petite
		{	vect = tab_test[i];								//nouvelle valeur pour vect
		}
	}
	return vect; //ne rend probablement pas le bon vecteur
}

Collider Collider::directionTo(Collider c)
{
	Vec2d to (c.getPosition());
	Collider coll (c.directionTo(to),c.getRadius());
	return coll;
}

double distanceTo(Vec2d to)
{
	return directionTo(to).lenght(); 	//besoin d'une methode applicquée à l'instance courante, pas le to, ne fonctionne pas
}

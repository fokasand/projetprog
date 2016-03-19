#include "Collider.hpp"
#include <iostream>
#include <Application.hpp>
#include <vector>
#include <utility>

//constructeur, verifier que r positif ou nul
Collider::Collider (Vec2d c, double r) :	
		centre(c),
		rayon(r)
{
	assert(rayon >=0); // gestion de l'erreur pour r negatif
}

//constructeur par copie
Collider::Collider (const Collider& a) :
	centre( a.getPosition()),
	rayon(a.getRadius())
	{}
	
//clamping

void Collider::clamping(Vec2d& c)
{
	Vec2d worldSize = getApp().getWorldSize();
	double width = worldSize.x;
	double height= worldSize.y; //on en a pas vraiment besoin si ? ben si banane sinon il sait pas ce que c'est width et height.
	
	while (c.x> width)
	{
		c.x -= worldSize.x;
	}
	
	while (c.y> height)
	{
		c.y -= worldSize.y;
	}
		while (c.x< 0)
	{
		c.x += worldSize.x;
	}
	
	while (c.y< 0)
	{
		c.y += worldSize.y;
	}
}

//getters

 const Vec2d& Collider::getPosition() const 
{
	return centre;
}

double Collider::getRadius() const
{
	return rayon;
}


//deplacement


double Collider::distanceTo(Vec2d to) const
{
	return directionTo(to).length();
}

Vec2d Collider::directionTo (Vec2d to) const
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
		{	//*dimension= new double(worldSize.x); Set all pointer to x coordinates
			dimension = new double(worldSize.x);
			coordinatei= new double(from.x);
			coordinatef= new double(to.x);
			
		}
		
		if (i==1)
		{	// Set all pointer to y coordinates
			*dimension = worldSize.y;
			*coordinatei = from.y;
			*coordinatef = to.y;	
		}
		
		if ((*coordinatei-*coordinatef)<(-*dimension/2)) //test si la coordonnee pointee de to n'est pas trop grande
		{						// si oui on la decremente de la taille du monde
			*coordinatef-=*dimension;
		}
		if ((*coordinatei-*coordinatef)>(*dimension/2)) //test si la coordonnee pointee de to n'est pas trop petite
								// si oui on l'incremente de la taille du monde
		{
			*coordinatef+=*dimension;
		}
	}
	
	Vec2d result (((from.x)-(to.x)),((from.y)-(to.y)));
	
	return result;
}


Vec2d Collider::directionTo (Collider c) const
{
	Vec2d to(c.getPosition());
	return 	directionTo(to);
}


//booléens

bool Collider::isColliderInside(const Collider& other) const
{
	if ((rayon >= other.getRadius()) and (other.distanceTo(getPosition())) <= (rayon-other.getRadius())) /*ici j hesite entre distanceTo(other.getPosition()) et other.distanceTo(*this), 
	je pense que les 2 marchent mais si jamais essaye l'autre. j'ai pas pu tester sans la fonction distanceTo */
	{
		return true; // si other est dans l'instance courante
	} else {
		return false;
	}
}

bool Collider::isColliding(const Collider& other) const
{
	if ((distanceTo(other.getPosition())) <= (rayon+other.getRadius()))
	{
		return true;
	} else
	{
		return false;
	}
}

bool Collider::isPointInside(const Vec2d& poin) const
{
	if (distanceTo(poin) <= rayon)
	{
		return true;
	}else
	{
		return false;
	}
}

//opérateurs:

bool Collider::operator>(const Collider& body2)
{
	return isColliderInside(body2);
}

bool Collider::operator|(const Collider& body2)//je suis pas sur qu'on puisse utiliser des méthodes sur des trucs si ils sont constants.
{
	return isColliding(body2);
}

bool Collider::operator>(const Vec2d& point)
{
	return isPointInside(point);
}

std::ostream& Collider::affiche(std::ostream& sortie) const
{
sortie << "Collider: position = " << getPosition() << "radius =" << getRadius() << std:: endl;

return sortie;
}

std::ostream& operator<<(std::ostream& sortie, Collider const& c)
{
return c.affiche(sortie);
}

Collider& Collider:: operator=(Collider b)
{
	swap(*this,b);
	return *this;
}

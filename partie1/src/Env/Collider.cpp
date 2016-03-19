#include "Collider.hpp"
#include <iostream>
#include <Application.hpp>
#include <vector>
#include <utility>

//constructeur, verifier que r positif ou nul
Collider::Collider (const Vec2d& c, const double& r) :	
		centre(c),
		rayon(r)
{
	assert(rayon >=0); // gestion de l'erreur pour r negatif
	clamping();
}

//constructeur par copie
Collider::Collider (const Collider& a) :
	centre( a.getPosition()),
	rayon(a.getRadius())
	{}
	

 //clamping
 
 void Collider::clamping() //takes current Collider and operates on its center
 {	
 	Vec2d worldSize = getApp().getWorldSize();
 	double width = worldSize.x;
 	double height= worldSize.y; //on en a pas vraiment besoin si ? ben si banane sinon il sait pas ce que c'est width et height.
 					// mais ça sert a rien de continuer à utiliser worldSize dans les calculs alors utiliser soit que height et width ou woldSize.x ou y direct
					// oui c'est vrai on peut mettre que worldsize dans les  comparateurs je pense
	while (centre.x> width)
 	{
		centre.x -= worldSize.x;
 	}
 	
	while (centre.y> height)
 	{
		centre.y -= worldSize.y;
 	}
 		while (centre.x< 0)
 	{
		centre.x += worldSize.x;
 	}
 	
	while (centre.y< 0)
 	{
		centre.y += worldSize.y;
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
	double width = worldSize.x;
	double height= worldSize.y;
	//creer deux pointeurs
	
	for (int i(0); i <2 ; i++)
	{
		if (i==0)
		{	//*dimension= new double(worldSize.x); Set all pointer to x coordinates
			dimension = &width;
			coordinatei= &from.x;
			coordinatef= &to.x;
			
		}
		
		if (i==1)
		{	// Set all pointer to y coordinates
			dimension = &height;
			coordinatei = &from.y;
			coordinatef = &to.y;	
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
	
	delete dimension;
	delete coordinatei;
	delete coordinatef;
	dimension=nullptr;
	coordinatei=nullptr;
	coordinatef=nullptr;
	
	Vec2d result (((from.x)-(to.x)),((from.y)-(to.y)));
	
	return result;
}


Vec2d Collider::directionTo (const Collider& c) const
{
	Vec2d to(c.getPosition());
	return 	directionTo(to);
}

void Collider::move(const Vec2d& dx)
{
	centre+=dx;
	clamping();
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

Collider& Collider::operator+=(const Vec2d& b)
{	
	move(b);
	return *this;
}

bool Collider::operator>(const Collider& body2) const
{
	return isColliderInside(body2);
}

bool Collider::operator|(const Collider& body2) const
{
	return isColliding(body2);
}

bool Collider::operator>(const Vec2d& point) const
{
	return isPointInside(point);
}

Collider& Collider:: operator=(Collider b)
{
	std::swap(*this,b);
	return *this;
}

//opérateur <<

std::ostream& Collider::affiche(std::ostream& sortie) const
{
sortie << "Collider: position : x=" << getPosition().x <<"y=" << getPosition().y << "radius =" << getRadius() << std:: endl;

return sortie;
}

std::ostream& operator<<(std::ostream& sortie, const Collider& c)
{
return c.affiche(sortie);
}




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
    clamping_(centre);
}

void clamping_(Vec2d& position) //méthode créée en dehors de collider pour pouvoir être utilisée sans créer de collider
{
	Vec2d worldSize = getApp().getWorldSize();
	double width = worldSize.x;
    double height= worldSize.y;
    
    while (position.x> width) {
        position.x -= worldSize.x;
    }

    while (position.y> height) {
        position.y -= worldSize.y;
    }
    while (position.x< 0) {
        position.x += worldSize.x;
    }

    while (position.y< 0) {
        position.y += worldSize.y;
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

double Collider::distanceTo(Collider c) const
{
    return distanceTo(c.getPosition());
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

    for (int i(0); i <2 ; i++) {
        if (i==0) {
            //dimension= new double(worldSize.x); Set all pointer to x coordinates
            dimension = &width;
            coordinatei= &from.x;
            coordinatef= &to.x;
        }
        if (i==1) {
            // Set all pointer to y coordinates
            dimension = &height;
            coordinatei = &from.y;
            coordinatef = &to.y;
        }
        if ((*coordinatei-*coordinatef)<(-*dimension/2)) { //test si la coordonnee pointee de to n'est pas trop grande
            // si oui on la decremente de la taille du monde
            *coordinatef= *coordinatef-*dimension;
        }
        if ((*coordinatei-*coordinatef)>(*dimension/2)) //test si la coordonnee pointee de to n'est pas trop petite
            // si oui on l'incremente de la taille du monde
        {
            *coordinatef= *coordinatef+*dimension;
        }
    }
    dimension=nullptr;
    coordinatei=nullptr;
    coordinatef=nullptr;

    Vec2d result (((to.x)-(from.x)),((to.y)-(from.y)));
    return result;
}

Vec2d Collider::directionTo (Collider c) const
{
    return 	directionTo(c.getPosition());
}

void Collider::move(const Vec2d& dx)
{
    centre+=dx;
    clamping();
}

//booléens

bool Collider::isColliderInside(const Collider& other) const
{
    if ((rayon >= other.getRadius()) and ((distanceTo(other)) <= (rayon-other.getRadius()))) {
        return true; // si other est dans l'instance courante
    } else {
        return false;
    }
}


bool Collider::isColliding(const Collider& other) const
{
    if ((distanceTo(other)) <= (rayon+other.getRadius())) {
        return true; // si other est en collision avec l'instance courante
    } else {
        return false;
    }
}

bool Collider::isPointInside(const Vec2d& poin) const
{
    if (distanceTo(poin) <= rayon) {
        return true;
    } else {
        return false;
    }
}

//opérateurs:

void operator+=(Collider& c,const Vec2d& b)
{
    c.move(b);
}

bool operator|(const Collider& body1,const Collider& body2)
{
    return body1.isColliding(body2);
}
bool operator>(const Collider& body1,const Collider& body2)
{
    return body1.isColliderInside(body2);
}
bool operator>(const Collider& body, const Vec2d& point)
{
    return body.isPointInside(point);
}

Collider& Collider:: operator=(Collider b)
{
    swap(*this,b);
    return *this;
}

void swap(Collider& a, Collider& b)
{
    std::swap(a.centre.x,b.centre.x);
    std::swap(a.centre.y,b.centre.y);
    std::swap(a.rayon,b.rayon);
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

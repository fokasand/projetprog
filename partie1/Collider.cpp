#include "Collider.hpp"
#include <iostream>
#include <Application.hpp>
#include <vector>
#include <utility>

//constructeur: prend un rayon et un centre en arguments
Collider::Collider (const Vec2d& c, const double& r) :
    centre(c),
    rayon(r)
{
    assert(rayon >=0); // gestion de l'erreur pour r negatif
    clamping(); //intialiser le collider dans la map
}

//constructeur par copie
Collider::Collider (const Collider& a) :
    centre( a.getPosition()),
    rayon(a.getRadius())
{}


//clamping, remet le centre du collider dans la map
void Collider::clamping()
{
    clamping_(centre);
}

 //fonction créée en dehors de collider pour pouvoir être utilisée sans créer de collider
void clamping_(Vec2d& position)
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
//rend la position du centre
const Vec2d& Collider::getPosition() const
{
    return centre;
}

//rend le rayon
double Collider::getRadius() const
{
    return rayon;
}


//deplacement
//rend la distance entre le collider et un vecteur
double Collider::distanceTo(Vec2d to) const
{
    return directionTo(to).length();
}

//rend la distance entre un collider et un autre collider
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

//une boucle pour le travail sur le coordonnées en x et une pour y
//puisque les traitements sont les mêmes

    for (int i(0); i <2 ; i++) {
        if (i==0) {
           //tous les pointeurs sur coordonnées en x
            dimension = &width;
            coordinatei= &from.x;
            coordinatef= &to.x;
        }
        if (i==1) {
            //pointeurs sur cordonnées en y 
            dimension = &height;
            coordinatei = &from.y;
            coordinatef = &to.y;
        }
        
        //test si la coordonnee pointee de to n'est pas trop grande
        if ((*coordinatei-*coordinatef)<(-*dimension/2)) 
        { 
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

//directionTo utilisé sur un collider directement
Vec2d Collider::directionTo (Collider c) const
{
    return directionTo(c.getPosition());
}

//bouge le centre du collider
void Collider::move(const Vec2d& dx)
{
    centre+=dx;
    clamping();
}

//booléens
//indique si un collider se trouve à l'interieur d'un autre
bool Collider::isColliderInside(const Collider& other) const
{
    if ((rayon >= other.getRadius()) and ((distanceTo(other)) <= (rayon-other.getRadius()))) {
        return true; 
    } else {
        return false;
    }
}

//indique si un Collider en touche un autre
bool Collider::isColliding(const Collider& other) const
{
    if ((distanceTo(other)) <= (rayon+other.getRadius())) {
        return true; // si other est en collision avec l'instance courante
    } else {
        return false;
    }
}

//teste si la position en argument se trouve dans la collider
bool Collider::isPointInside(const Vec2d& point) const
{
    if (distanceTo(point) <= rayon) {
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
    sortie << "Collider: position : x=" << centre.x <<"y=" << centre.y << "radius =" << rayon << std:: endl;

    return sortie;
}

std::ostream& operator<<(std::ostream& sortie, const Collider& c)
{
    return c.affiche(sortie);
}

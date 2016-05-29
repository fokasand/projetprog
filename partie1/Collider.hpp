#include <cassert>
#ifndef COLLIDER_H
#define COLLIDER_H
#include "Utility/Vec2d.hpp"

using namespace std;

class Collider
{

public :

    friend void swap(Collider& a, Collider& b);

    //constructeur, verifier que r positif ou nul,
    Collider (const Vec2d& c,const double& r);
    Collider (const Collider& a);

    //getters
    const Vec2d& getPosition() const;
    double getRadius() const;

    //déplacement
    Vec2d directionTo (Collider c) const;
    Vec2d directionTo (Vec2d to) const;
    void move(const Vec2d& dx);

    // distances

    double distanceTo(Vec2d to) const;
    double distanceTo(Collider c) const;

    //clamping
    void clamping();

    // booléens

    bool isColliderInside(const Collider& other) const;
    bool isColliding(const Collider& other) const;
    bool isPointInside(const Vec2d& point) const;

    // =
    Collider& operator=(Collider b);

    //aide pour <<

    std::ostream& affiche(std::ostream& sortie) const;
private :
    Vec2d centre;
    double rayon;
};

//opérateurs externes:
std::ostream& operator<<(std::ostream& sortie, const Collider& c);
bool operator>(const Collider& body, const Vec2d& point);
bool operator>(const Collider& body1,const Collider& body2);
bool operator|(const Collider& body1,const Collider& body2);
void operator+=(Collider& c,const Vec2d& b);

//aide pour =
void swap(Collider& a, Collider& b);

void clamping_(Vec2d& pos);

#endif

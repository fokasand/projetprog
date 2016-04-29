#include "Flower.hpp"
#include "Env.hpp"


//constructeur de flower avec 3 paramètres, initialise le collider,
//quantité de pollen et texture de la fleur à l'aide de l'indice max
Flower::Flower (const Vec2d& c, const double& r, double qpollen)
    : Collider::Collider(c, r), pollen(qpollen), max(getFlower()["textures"].size()-1),
      texture(getAppTexture(getFlower()["textures"][uniform(0,max)].toString()))
{
    (*this).clamping(); // ne suffit pas. pourquoi?
}

//retrait de pollen tant qu'il en reste
double Flower::takePollen (double take)
{
    if ( pollen >= take) {
        pollen-= take; // on rend la quantité prélevée
        // si superieure à celle de pollen
    } else {
        take= pollen; //on rend la quantité disponible
        pollen-= take;
    }
    return take;
}

//dessiner les fleurs
void Flower::drawOn(sf::RenderTarget& target) const
{

    auto flowerSprite = buildSprite(centre, rayon, texture);
    target.draw(flowerSprite);

}

//racourci pour les données de configuration
j::Value getFlower()
{
    return getAppConfig()["simulation"]["flower"];
}

void Flower::update(sf::Time dt)
{
    if (pollen<= 0) 
    {
		cout << " bonjour" << endl;
        getAppEnv().killFlower(); // dit qu'une fleur est morte, pour que Env regarde laquelle est morte et l'enlève du tableau.
    } else 
    {
        pollen += dt.asSeconds() * log( getAppEnv().world_.humid(centre) / getFlower()["growth"]["threshold"].toDouble() );
        if (pollen >= getFlower()["growth"]["split"].toDouble()) {
            Vec2d pp;
            int essaismax(0);
            do {
                double d (uniform(0.5*rayon, 2*rayon));
                pp = centre + Vec2d::fromRandomAngle() * d;
                ++essaismax;
            } while ( (!getAppEnv().addFlowerAt(pp)) and (essaismax <100) ); // dans add flower at, rajouter un test que 2 fleurs ne se collident pas.

            pollen = pollen/2;
        }
    }
}

double Flower::getPollen() const
{
    return pollen;
}

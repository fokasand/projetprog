#include "Flower.hpp"
#include "Env.hpp"
#include <Random/Random.hpp>

//l'indice max du teableau de textures est le même pour toutes les fleurs
const int Flower::max = getFlower()["textures"].size()-1;
//$taux de croissance
const double Flower::growth_lim = getFlower()["growth"]["threshold"].toDouble();
//quantité de pollen minimale pour se diviser
const double Flower::pollen_min = getFlower()["growth"]["split"].toDouble();
 
//constructeur de flower avec 3 paramètres, initialise le collider,
//quantité de pollen et texture de la fleur à l'aide de l'indice max
Flower::Flower (const Vec2d& c, const double& r, double qpollen)
    : Collider::Collider(c, r), pollen(qpollen),
      texture(getAppTexture(getFlower()["textures"][uniform(0,max)].toString()))
{} // la fleur est clampée automatiquement

//retrait de pollen tant qu'il en reste
double Flower::takePollen (double take)
{
	if (pollen > 0 )
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
	return 0;
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
        getAppEnv().killFlower(); // dit qu'une fleur est morte, pour que une méthode dans Env regarde laquelle est morte et l'enlève du tableau.
    } else 
    {
        pollen += dt.asSeconds() * log( getAppEnv().world_.howhumid(centre) /growth_lim  );
        if (pollen >= pollen_min) 
        {
            Vec2d pp;
            int essaismax(0);
            do {
                double d (uniform(0.5*rayon, 2*rayon));
                pp = centre + Vec2d::fromRandomAngle() * d;
                ++essaismax;
            } while ( (!getAppEnv().addFlowerAt(pp)) and (essaismax <=100) );
			if (essaismax != 100) // la quantité de pollen n'est divisée par 2 que si on a bien fait la fleur.
			{
            pollen = pollen/2;
			}
        }
    }
}

double Flower::getPollen() const
{
    return pollen;
}

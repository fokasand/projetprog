#include "Flower.hpp"
#include "Env.hpp"
#include <Random/Random.hpp>

//constructeur de flower avec 3 paramètres, initialise le collider,
//quantité de pollen et texture de la fleur à l'aide de l'indice max
Flower::Flower (const Vec2d& c, const double& r, double qpollen)
    : Collider::Collider(c, r), pollen(qpollen), max(getFlower()["textures"].size()-1),
      texture(getAppTexture(getFlower()["textures"][uniform(0,max)].toString())),
      split_(getFlower()["growth"]["split"].toDouble()),
      threshold_(getFlower()["growth"]["threshold"].toDouble())
{} // la fleur est clampée automatiquement par le constructeur de Collider

//retrait de pollen tant qu'il en reste
double Flower::takePollen (double take)
{
	if (pollen > 0 )
	{
		//si il y a assez de pollen pour prélever la quantité take
		if ( pollen >= take) {
			pollen-= take; // on rend la quantité prélevée
			
		} else // si superieure à celle de pollen
		double taken;
		{
			taken= pollen; //on rend la quantité disponible
			pollen-= taken; //le pollen est à 0
		}
		return taken;
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
	//si la fleur n'a plus de pollen le tuer
    if (pollen<= 0) 
    {
    	// dit qu'une fleur est morte, pour que une méthode dans Env regarde laquelle est morte et l'enlève du tableau.
    	 getAppEnv().killFlower(); 
    } else 
    {
    	//le pollen augmente
        pollen += dt.asSeconds() * log( getAppEnv().world_.howhumid(centre) / threshold_ );
        
        //si assez de pollen, la fleur se multiplie
        if (pollen >= split_) 
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

//getter 
double Flower::getPollen() const
{
    return pollen;
}

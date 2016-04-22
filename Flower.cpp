#include "Flower.hpp"

//constructeur de flower avec 3 paramètres, initialise le collider, 
//quantité de pollen et texture de la fleur à l'aide de l'indice max
Flower::Flower (const Vec2d& c, const double& r, double qpollen) 
: Collider::Collider(c, r), pollen(qpollen), max(getFlower()["textures"].size()-1),
texture(getAppTexture(getFlower()["textures"][uniform(0,max)].toString()))
{}
//retrait de pollen tant qu'il en reste 
double Flower::takePollen (double take)
{
	if ( pollen >= take)
	{
		pollen-= take; // on rend la quantité prélevée
					   // si superieure à celle de pollen
	}else {	
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
	if (pollen > 0)
	{
		pollen += dt.asSeconds() * log(getAppEnv().world_.humid(centre) / getFlower()["growth"]["threshold"].toDouble());
	}
	std::cout << pollen << " " << dt.asSeconds()* log(getAppEnv().world_.humid(centre) / getFlower()["growth"]["threshold"].toDouble()) <<std::endl;
	if (pollen >= getFlower()["growth"]["split"].toDouble())
	{
		Vec2d pp;
		do
		{
			double d (uniform(0.5*rayon, 2*rayon));
			pp = centre + Vec2d::fromRandomAngle() * d;
		} while (!getAppEnv().addFlowerAt(pp));
		pollen = pollen/2;
	}
}

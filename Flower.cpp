#include "Flower.hpp"
#include <Application.hpp>
#include <Random/Random.hpp>

//constructeur de flower avec 3 paramètres
Flower::Flower (const Vec2d& c, const double& r, double qpollen) 
: Collider::Collider(c, r), pollen(qpollen), 
texture(getAppTexture(["simulation"]["flower"]["textures"][uniform(0,["simulation"]["flower"]["textures"].size()-1)].toString()))
{}

//retrait de pollen tant qu'il en reste 
double Flower::takePollen (double take)
{
	if ( pollen >= take)
	{
		pollen-= take; // on rend la quantité prélevée
					   // si superieure à celle de pollen
	}else {	
		take = 0 ;		// sinon on rend 0 (n'affecte pas take car 
						// le passage ne se fait pas par ref)
	}
	return take;
}

//dessiner les fleurs
void Flower::drawOn(sf::RenderTarget& target) const 
{
	
	auto flowerSprite = buildSprite(centre, rayon, texture);
      target.draw(flowerSprite);
	
}

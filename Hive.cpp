#include "Hive.hpp"
#include <SFML/Graphics.hpp>
#include "Utility/Utility.hpp"
#include "Env.hpp"
#include <Application.hpp>
#include "Bee.hpp"

//constructeur 
Hive::Hive(const Vec2d& c) 
: Collider::Collider(c,getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble()),
nectar_(getHive()["initial"]["nectar"].toDouble()),
texture(getAppTexture(getHive()["texture"].toString()))
{}

//destructeur
Hive::~Hive()
{
	clearBees();
}

//redefinir le dessin pour les ruches
void Hive::drawOn(sf::RenderTarget& targetWindow) const
{

	auto hiveSprite = buildSprite(centre, rayon*2.5, texture);
      targetWindow.draw(hiveSprite);	
      
      if(isDebugOn())
      {
		Vec2d curseur (getApp().getCursorPositionInView());
		double x = getAppEnv().world_.toGrid(curseur.x);
		double y = getAppEnv().world_.toGrid(curseur.y);
		if ((x < getAppEnv().world_.getnbCells_()) and (x >= 0) and (y < getAppEnv().world_.getnbCells_()) and (y >=0) and (*this > curseur))
		{
			Vec2d affiche (curseur.x +60,curseur.y );
			auto const text = buildText(to_nice_string(nectar_), affiche , getAppFont(), 30, sf::Color::Green);
			targetWindow.draw(text);
		}
		  
	  }
}
	
void Hive::dropPollen(double qte)
{
		
}
	
void Hive::takeNectar(double qte)
{
	
}
		
//racourci pour les données de configuration
j::Value getHive()
{
	return getAppConfig()["simulation"]["hive"];
}

void Hive::update(sf::Time dt)
{
	
}

//efface les abeilles mortes
void Hive::killBee()
{
	for (size_t i(0); i < bees_.size() ; ++i) {
        if (bees_[i]->isDead()) {
            delete bees_[i];
            bees_[i]= nullptr;
        }
    }

    bees_.erase(std::remove(bees_.begin(), bees_.end(), nullptr), bees_.end());
}

//efface les abeilles et nettoie le tableau
void Hive::clearBees()
{
		for (size_t i(0); i < bees_.size(); ++i)
	{
		delete bees_[i];
		bees_[i] = nullptr;
	}
	bees_.clear();	
}

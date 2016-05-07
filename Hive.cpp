#include "Hive.hpp"
#include <SFML/Graphics.hpp>
#include "Utility/Utility.hpp"
//probablement une dépendance circulaire qvec World
#include "Env.hpp"

//constructeur 
Hive::Hive(const Vec2d& c) 
: Collider::Collider(c,getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble()),
nectar_(getHive()["initial"]["nectar"].toDouble()),
texture(getAppTexture(getHive()["texture"].toString()))
{cerr << "une ruche est crée"<< std::endl;}

//destructeur
Hive::~Hive()
{
	for (size_t i(0); i < bees_.size(); ++i)
	{
		delete bees_[i];
		bees_[i] = nullptr;
	}
	bees_.clear();
}

//redefinir le dessin pour les ruches
void Hive::drawOn(sf::RenderTarget& targetWindow) const
{
	cerr << "on dessine" << std::endl;
	auto hiveSprite = buildSprite(centre, rayon*2.5, texture);
      targetWindow.draw(hiveSprite);	
      
      if(isDebugOn())
      {
		 double x = getAppEnv().world_.toGrid(getApp().getCursorPositionInView().x);
		double y = getAppEnv().world_.toGrid(getApp().getCursorPositionInView().y);
		if ((x < getAppEnv().world_.getnbCells_()) and (x >= 0) and (y < getAppEnv().world_.getnbCells_()) and (y >=0))
		{
			Vec2d affiche (getApp().getCursorPositionInView().x -60,getApp().getCursorPositionInView().y );
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

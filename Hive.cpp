#include "Hive.hpp"

//racourci pour les données de configuration
j::Value getHive()
{
	return getAppConfig()["simulation"]["hive"];
}
//constructeur 
Hive::Hive(const Vec2d& c) 
: Collider(c,getAppConfig()["simulation"]["env"]["initial"]["hive"]["manual"].toDouble), nectar_(getHive()["initial"]["nectar"].toDouble()),
texture(getAppTexture(getHive()["textures"].toString)
{}

//destructeur
~Hive::Hive()
{
	for (size_t i(0); i < bees_.size(); ++i)
	{
		delete bees_[i]:
		bees_[i] = nullptr;
	}
	bees_.clear();
}
//redefinir le dessin pour les ruches
void Hive::drawOn(sf::RenderTarget& targetWindow) const override
{
	auto hiveSprite = buildSprite(centre, rayon*2.5, texture);
      target.draw(hiveSprite);
      
      if(isDebugOn())
      {
		 double x = toGrid(getApp().getCursorPositionInView().x);
		double y = toGrid(getApp().getCursorPositionInView().y);
		if ((x < nbCells_) and (x >= 0) and (y < nbCells_) and (y >=0))
		{
			Vec2d affiche (getApp().getCursorPositionInView().x -60,getApp().getCursorPositionInView().y );
			auto const text = buildText(to_nice_string(nectar_, affiche , getAppFont(), 30, sf::Color::Green);
			target.draw(text);
		}
		  
	  }
}
	
void Hive::dropPollen(double qte)
{
		
}
	
void Hive::takeNectar(double qte)
{
	
}
		

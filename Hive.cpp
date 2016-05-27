#include <SFML/Graphics.hpp>
#include "Utility/Utility.hpp"
#include "Env.hpp"
#include <Application.hpp>
#include "Bee.hpp"
#include "WorkerBee.hpp"
#include "ScoutBee.hpp"

//constructeur 
Hive::Hive(const Vec2d& c) 
: Collider::Collider(c,getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble()),
nectar_(getHive()["initial"]["nectar"].toDouble()),
texture(getAppTexture(getHive()["texture"].toString())),
nectar_lim(getHive()["reproduction"]["max bees"].toDouble()),
max_bees(getHive()["reproduction"]["max bees"].toInt()),
proba(getHive()["reproduction"]["scout probability"].toDouble())
{}

//destructeur
Hive::~Hive()
{
	clearBees();
}


//ajoute des abeilles aléatoirement
void Hive::addRandom()
{
	if ((nectar_>= nectar_lim) and (bees_.size() < max_bees))
	{
		if( bernoulli(proba))
		{
			addWorker();
		} else {
			
			addScout();
		}
	}
}
//redefinir le dessin pour les ruches
void Hive::drawOn(sf::RenderTarget& targetWindow) const
{

	auto hiveSprite = buildSprite(centre, rayon*2.5, texture);
      targetWindow.draw(hiveSprite);	
      
     for (size_t i(0); i <bees_.size() ; ++i)
	{
		bees_[i]->drawOn(targetWindow);
	}
	
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
	nectar_+=qte;	
}
	
void Hive::takeNectar(double qte)
{
	nectar_-=qte;
}
		
//racourci pour les données de configuration
j::Value getHive()
{
	return getAppConfig()["simulation"]["hive"];
}

void Hive::update(sf::Time dt)
{
	vector<Bee*> copie = bees_;
	for (size_t i(0); i <copie.size() ; ++i)
	{
		copie[i]->update(dt);
		if (copie[i]->isDead()) 
		{
            delete copie[i];
            copie[i]=nullptr;
            bees_[i] = nullptr;
        }
	}
    bees_.erase(std::remove(bees_.begin(), bees_.end(), nullptr), bees_.end());
	getBeesInHive();
	beesInteract();
	addRandom();
}

/*
//ajouter une abeille à la ruche
void Hive::addBee()
{
	bees_.push_back(new Bee({centre},30,this,250,500));
}
* */

//rend un tableau avec les abeilles dans la ruche
void Hive::getBeesInHive()
{
	waiting_list.clear();
	for (auto bee: bees_)
	{
		// le state d'indice 0 dans states_  est IN_HIVE
		if (bee->getState()==0) // peut etre mieux avec collider
		{
			waiting_list.push_back(bee);
		}
	}	
}

void Hive::beesInteract()
{
	for(int i(0); i< waiting_list.size(); ++i)
	{

		for (int j(0); j< waiting_list.size(); ++j)
		{
			if (i!=j)
			{
			bees_[j]->interact(bees_[i]);
			}
		}
	}
}

//ajouts spécifiques:
WorkerBee* Hive::addWorker()
{
	Vec2d alea ( uniform(-rayon,rayon), uniform ( -rayon,rayon));
	WorkerBee* ptr (new WorkerBee( {centre + alea}, this));
	bees_.push_back(ptr);
	return ptr;
}

ScoutBee* Hive::addScout()
{
	Vec2d alea ( uniform(-rayon,rayon), uniform ( -rayon,rayon));
	ScoutBee* ptr (new ScoutBee( {centre + alea}, this));
	bees_.push_back(ptr);
	return ptr;
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
	waiting_list.clear();
}

double Hive::getNectar() const
{
	return nectar_;
}

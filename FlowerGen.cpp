#include "FlowerGen.hpp"
#include "Env.hpp"

void FlowerGenerator::update(sf::Time dt)
{
	t+=dt;
	if ((t >= sf::seconds(getAppConfig()["simulation"]["flower generator"]["delay"].toDouble())) and 
	(getAppConfig()["simulation"]["flower generator"]["active"].toBool()))
	{
		reset();
		Vec2d p(uniform(0.0, getApp().getWorldSize().x), uniform(0.0, getApp().getWorldSize().y));
		getAppEnv().addFlowerAt(p); // mais si ca marche pas parce que c'est du rocher? osef?
	}
}

void FlowerGenerator::reset()
{
	t=sf::Time::Zero;
}

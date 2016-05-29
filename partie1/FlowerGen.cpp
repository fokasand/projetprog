#include "FlowerGen.hpp"
#include "Env.hpp"
#include <Random/Random.hpp>
#include <Application.hpp>


void FlowerGenerator::update(sf::Time dt)
{
    t+=dt;
    if ((t >= sf::seconds(getAppConfig()["simulation"]["flower generator"]["delay"].toDouble())) and //vérifie que le temps est écoulé, et que la génération aléatoire est activée
        (getAppConfig()["simulation"]["flower generator"]["active"].toBool())) {
        reset(); // remet le compteur à zéro
        Vec2d p(uniform(0.0, getApp().getWorldSize().x), uniform(0.0, getApp().getWorldSize().y));
        getAppEnv().addFlowerAt(p);
    }
}

void FlowerGenerator::reset()
{
    t=sf::Time::Zero;
}

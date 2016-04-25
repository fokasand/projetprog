#ifndef FLOWERGEN_H
#define FLOWERGEN_H
#include <SFML/Graphics.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>


class FlowerGenerator
{
public :
	
	void update(sf::Time dt);
	void reset();
	
private :

	sf::Time t;

};
#endif


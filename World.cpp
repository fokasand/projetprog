#include "World.hpp"
#include <iostream>
#include <fstream>
#include "Env.hpp"
#include <Random/Random.hpp>
#include <Application.hpp>

//raccourci
j::Value getTerrain()
{
    return getAppConfig()["simulation"]["world"];
}

//initialisation ensemble de textures
void World::reloadCacheStructure()

{
    //vector contenant l'ensemble de sommets
    std::vector<sf::Vertex> Vertexes (generateVertexes(getTerrain()["textures"],nbCells_ ,cellSize_));

    //initialisation à tous les sommets de la grille
    grassVertexes_=Vertexes;
    waterVertexes_=Vertexes;
    rockVertexes_=Vertexes;
    humidityVertexes_ =Vertexes;

    //initialisation de la texture à afficher
    renderingCache_.create(nbCells_*cellSize_, nbCells_*cellSize_);
    humideCache_.create(nbCells_*cellSize_, nbCells_*cellSize_);
}

//fonction draw
void World::drawOn(sf::RenderTarget& target) const
{
    if (getTerrain()["show humidity"].toBool()) {
        sf::Sprite cachehumide(humideCache_.getTexture());
        target.draw(cachehumide);
    } else {
        sf::Sprite cache(renderingCache_.getTexture());
        target.draw(cache);
    }

    //activer le mode debugeur de humidity
    if(isDebugOn()) {
        double x = toGrid(getApp().getCursorPositionInView().x);
        double y = toGrid(getApp().getCursorPositionInView().y);
        if ((x < nbCells_) and (x >= 0) and (y < nbCells_) and (y >=0)) {
            Vec2d affiche (getApp().getCursorPositionInView().x -60,getApp().getCursorPositionInView().y );
            auto const text = buildText(to_nice_string(humide_[toUnid(x,y)]), affiche , getAppFont(), 30, sf::Color::Red);
            target.draw(text);
        }

    }
}

//mettre a jour rendering_Cache et humide cache
void World::updateCache()
{
    //nettoyage des textures à afficher
    renderingCache_.clear();
    humideCache_.clear();

    //calcule les niveaux d'humidité max et min
    minmaxhumid();

    //dessine les textures dans renderingCache_
    sf::RenderStates rsrock;
    sf::RenderStates rsgrass;
    sf::RenderStates rswater;

    sf::RenderStates rshumide;

    rsrock.texture = &getAppTexture(getTerrain()["textures"]["rock"].toString()); // ici pour la texture liée à la tex
    rsgrass.texture = &getAppTexture(getTerrain()["textures"]["grass"].toString());
    rswater.texture = &getAppTexture(getTerrain()["textures"]["water"].toString());

    //parcour chaque cellule une à une
    for(size_t j(0); j<cells_.size(); ++j) {
        //contient les coordonnées dans sommets
        std::vector<std::size_t> indexes_for_cell (indexesForCellVertexes(toBid(j).x, toBid(j).y, nbCells_ ));
        if (cells_[j] == Kind::rock) {
            for (int i = 0; i <4 ; ++i) {
                //pour chaque sommet, colorie pour la texture appropriée
                rockVertexes_[indexes_for_cell[i]].color.a = 255;
                grassVertexes_[indexes_for_cell[i]].color.a = 0;
                waterVertexes_[indexes_for_cell[i]].color.a = 0;
            }
        }

        if (cells_[j] == Kind::grass) {
            for (int i = 0; i <4 ; ++i) {
                rockVertexes_[indexes_for_cell[i]].color.a = 0;
                grassVertexes_[indexes_for_cell[i]].color.a = 255;
                waterVertexes_[indexes_for_cell[i]].color.a = 0;
            }
        }

        if (cells_[j] == Kind::water) {
            for (int i = 0; i <4 ; ++i) {
                rockVertexes_[indexes_for_cell[i]].color.a = 0;
                grassVertexes_[indexes_for_cell[i]].color.a = 0;
                waterVertexes_[indexes_for_cell[i]].color.a = 255;
            }
        }

        double niveaubleu (((humide_[j] - minHumidity)/ (maxHumidity - minHumidity)) * 255);
        for (int i(0); i<4; ++i) {
            humidityVertexes_[indexes_for_cell[i]].color = sf::Color(0, 0, niveaubleu);
        }

    }

    // dessin de toutes les textures dans le cache à afficher
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rsrock);
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rsgrass);
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rswater);
    humideCache_.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads, rshumide);
    //affichage du cache
    renderingCache_.display();
    humideCache_.display();
}

void World::reset(bool regenerate=true)
{
    if (regenerate) {
        reloadConfig();
        reloadCacheStructure();
        seeds_.clear();

        //initialisation de seeds_ avec les graines
        //ajout dans seeds_ d'une graine aux coordonnées aléatoires tant que le nombre de graines maximal n'est pas atteint
        for (int i(0); i < nbGrass_ ; ++i) {
            //coordonnées aléatoires
            sf::Vector2i coord (uniform(0, nbCells_-1) ,uniform(0, nbCells_-1));
            Seed graine = {coord,Kind::grass};
            seeds_.push_back(graine);
        }

        // d'abord grass puis water pour que l'eau ne soit pas écrasée si jamais il y avait 2 graines ayant les mêmes coordonnées

        for (int i(0); i < nbWater_ ; ++i) {
            sf::Vector2i coord (uniform(0, nbCells_-1),uniform(0, nbCells_-1));
            Seed graine = {coord,Kind::water};
            seeds_.push_back(graine);

            humidcalc(toUnid(coord.x,coord.y)); // fais pour 1 graine.
        }

        //affecte la texture de la graine à la cellule pour chaque graine de seeds_
        for (size_t i(0); i <seeds_.size() ; ++i) {
            seedTocell(i);
        }

        steps(getTerrain()["generation"]["steps"].toInt(),false); //false est par défaut normalement, pk ne marche pas ?
        smooths(getTerrain()["generation"]["smoothness"]["level"].toInt(),false);

        updateCache();
    }

    else

    {
        reloadConfig();
        reloadCacheStructure();
        updateCache();
    }
}


void World::reloadConfig()
{

    nbCells_ = getTerrain()["cells"].toInt();
    cellSize_ = getTerrain()["size"].toDouble() / nbCells_;
    nbWater_ = getTerrain()["seeds"]["water"].toInt();
    nbGrass_ = getTerrain()["seeds"]["grass"].toInt();

    humidityRange_=0;
    e = getTerrain()["generation"]["humidity"]["initial level"].toDouble();
    l = getTerrain()["generation"]["humidity"]["decay rate"].toDouble();
    double t(getTerrain()["generation"]["humidity"]["threshold"].toDouble());

    while (e * exp(-humidityRange_ / l) > t) {
        ++humidityRange_;
    }
    cells_ = std::vector<Kind> (nbCells_*nbCells_, Kind::rock);
    humide_ = std::vector<double>(nbCells_*nbCells_, 0.0);
}

//humidité
void World::humidcalc(int pos)
{
    int xpos(toBid(pos).x);
    int ypos(toBid(pos).y);

    for (int x(-humidityRange_); x<=(humidityRange_+ 1); ++x) {
        for (int y(-humidityRange_); y<=(humidityRange_+ 1); ++y) {
            if (std::hypot(-x, -y) <= humidityRange_) {
                if ((xpos+x >= 0) and (ypos+y >= 0) and (xpos+x < nbCells_) and (ypos+y < nbCells_) and not((x==0) and y==0))
                    //modulariser cette partie dans un booléen que l'on utilisera aussi pour smooth?
                {
                    humide_[toUnid(xpos+x,ypos+y)] += e * exp(- std::hypot(-x, -y)  / l);
                }
            }
        }
    }
}

void World::minmaxhumid()
{
    minHumidity=10000000;
    maxHumidity=0;

    for (size_t j(0); j<humide_.size(); ++j) {
        if ( humide_[j] > maxHumidity) {
            maxHumidity = humide_[j];
        }
        if ( humide_[j] < minHumidity) {
            minHumidity = humide_[j];
        }
    }
}

// load la configuration de la carte depuis un fichier
void World::loadFromFile()
{
    reloadConfig();
    std::ifstream in;
    std::string i (getApp().getResPath()+getTerrain()["file"].toString());
    in.open(i);
    if (in.fail()) {
        throw std::runtime_error("AIEAIEAIEAIE");
    } else {

        in >> nbCells_;
        in >> std::ws;
        in >> cellSize_;
        in >> std::ws;
        for (size_t i (0); i < cells_.size() ; ++i) {

            in >> std::ws;
            short var;
            Kind type;
            in >> var;
            type = static_cast<Kind>(var);
            cells_[i] = type;
        }
        in >> std::ws; // pour sauter le retour a la ligne
        for (size_t i (0); i < humide_.size() ; ++i) {

            in >> std::ws;
            double var;
            in >> var;
            humide_[i] = var;
        }
    }
    reloadCacheStructure();
    updateCache();
}

//sauvegarde la configuration dans un fichier
void World::saveToFile()
{
    std::ofstream out;
    std::string i (getApp().getResPath()+getTerrain()["file"].toString());
    out.open(i);
    if (out.fail()) {
        throw std::runtime_error("fichier non ouvert");
    } else {

        out << nbCells_ << std::endl;
        out << cellSize_ << std::endl;
        for (size_t i (0); i < cells_.size() ; ++i) {
            short var (static_cast<short>(cells_[i]));
            out << var << " ";
        }
        out << std::endl;
        for (size_t i (0); i < humide_.size() ; ++i) {
            double var(humide_[i]);
            out << var << " ";
        }
    }

}

// mvt des graines
//un déplacement
void World::step()
{
    double waterprob (getTerrain()["seeds"]["water teleport probability"].toDouble());
    for (size_t i(0); i < seeds_.size(); ++i) {
        //déplacement aléatoire des graines d'herbe ou eau qui ne se teleportent pas

        if (seeds_[i].nature == Kind::grass or (seeds_[i].nature == Kind::water and !bernoulli(waterprob))) {
            sf::Vector2i nouvelles (seeds_[i].coord.x+uniform(-1,1),seeds_[i].coord.y+uniform(-1,1)); //truc intermediaire à enlever

            //remettre la graine dans la fenetre si elle depasse
            debVect(nouvelles);
            seeds_[i].coord = nouvelles;
        } else {
            //la graine d'eau se teleporte
            sf::Vector2i nouvelles (uniform(0, nbCells_-1),uniform(0, nbCells_-1)); //à modulariser !!
            seeds_[i].coord = nouvelles ;
        }

        seedTocell(i);
    }
}

// i déplacements
void World::steps( int i, bool regeneration = false)
{
    for (int j(0); j < i; ++j) {
        step();
    }
    if (regeneration) {
        updateCache();
    }
}

//affecte la texture entourante majoritaire a chaque cellule
void World::smooth()
{
    auto copie_de_cells_ = cells_;

    double wsmooth (getTerrain()["generation"]["smoothness"]["water neighbourhood ratio"].toDouble());
    double gsmooth (getTerrain()["generation"]["smoothness"]["grass neighbourhood ratio"].toDouble());
    for (int x(0); x < nbCells_; ++x) {
        for (int y(0); y < nbCells_; ++y) {
            double Someau(0.0);
            double Somherbe(0.0);
            double Somtot(0.0);
            for (int h(-1); h<=1; ++h) {
                for (int v(-1); v<=1; ++v) {
                    if (not((v==0) and (h==0))) {
                        if ((x+h>=0) and (y+v>=0) and (y+v <=nbCells_-1) and (x+h<= nbCells_-1)) {

                            if (copie_de_cells_[toUnid(x+h,y+v)] == Kind::water) {
                                ++Someau;
                            }
                            if (copie_de_cells_[toUnid(x+h,y+v)] == Kind::grass) {
                                ++Somherbe;
                            }
                            ++Somtot;
                        }
                    }
                }
            }

            if ((Someau/Somtot) >= wsmooth ) {
                if (copie_de_cells_[toUnid(x,y)] != Kind::water) {
                    copie_de_cells_[toUnid(x,y)] = Kind::water;
                    humidcalc(toUnid(x,y));
                }
            }
            if ((Somherbe/Somtot) >= gsmooth) {
                if (copie_de_cells_[toUnid(x,y)] == Kind::rock) {
                    copie_de_cells_[toUnid(x,y)] = Kind::grass;
                }
            }
        }
    }
    std::swap(cells_, copie_de_cells_);
}

void World::smooths( int i, bool regeneration = false)
{
    for (int j(0); j < i; ++j) {
        smooth();
    }
    if (regeneration) {
        updateCache();
    }
}


//débordements de la fenetre d'affichage

void World::debSup (int& c)
{
    if (c>= nbCells_) {
        c =nbCells_-1;
    }
}

void World::debInf (int& c)
{
    if (c < 0) {
        c =0;
    }
}

void World::debVect (sf::Vector2i& coord)
{
    debSup(coord.x);
    debInf(coord.x);

    debSup(coord.y);
    debInf(coord.y);

}

//donner le type de la graine à la cellule ayant les mêmes coordonées
void World::seedTocell(size_t i)
{
    //sauf si la texture de la cellule est de l'eau
    if (cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] != Kind::water) {
        cells_[toUnid(seeds_[i].coord.x, seeds_[i].coord.y)] = seeds_[i].nature;
        if (seeds_[i].nature == Kind::water) {
            humidcalc(toUnid(seeds_[i].coord.x, seeds_[i].coord.y));
        }
    }
}

//fonctions conversions:
int World::toUnid (int x, int y) const
{
    if((y)*nbCells_+x <0) 
    {
		cout << x <<"," << y << " et nbcells = " << nbCells_ << endl;
        throw std::out_of_range( "les coordonnées reçues inférieures à l'indice minimum possible" );
    }
    if((y)*nbCells_+x > nbCells_*nbCells_) 
    {
        cout << x <<"," << y << " et nbcells = " << nbCells_ << endl;
        throw std::out_of_range( "les coordonnées reçues dépassent l'indice maximum possible:"  );
    }
    return y*nbCells_+x;
}

sf::Vector2i World::toBid( int x) const
{
    if ((x>= 0) and x < (nbCells_*nbCells_)) {
        sf::Vector2i retour;
        retour.x= x%nbCells_;
        retour.y= x/nbCells_;
        return retour;
    } else {
        throw std::out_of_range(" coordonnée unidimensionnelle ne fait pas partie du tableau.");
    }
}

//getter for cells
std::vector<Kind> World::getcells ()
{
    return cells_;
}
//convertir du graphique à tableau
// on peux faire une fonction prennant le vecteur comme argument directement
int World::toGrid(double p) const
{
    return p/cellSize_;
}

double World::howhumid (Vec2d const& p)
{
    return humide_[toUnid(toGrid(p.x),toGrid(p.y))];
}

//verifie que one fleur peut être plantée
bool World::isGrowable(const Vec2d&p) // ATTENTION, il peut tester des choses qui ne sont pas dans le monde, donc clamper avant de l'utiliser
{
    double width = getApp().getWorldSize().x;
    double height= getApp().getWorldSize().y;
    //verifier que le type de la cellule testée est de l'herbe, et que le point appartient au monde.
    if((p.x <  width) and (p.y< height) and (p.x> 0) and (p.y > 0))
    {
		if (cells_[toUnid(toGrid(p.x),toGrid(p.y))]== Kind::grass) // pour que tounid ne soit pas appelé avec des coordonnées incorrectes, on le met dans la condition
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
	return false ;
	}
}

int World::getnbCells_()const
{
	return nbCells_;
}

bool World::isHiveable(const Vec2d& position, double radius)
{
	for (int xpos(position.x-radius); xpos < position.x+radius ; ++xpos)
	{
		for (int ypos(position.y-radius); ypos < position.y+radius ; ++ypos)
		{
			Vec2d v (xpos,ypos);
			clamping_(v);
			if (!isGrowable(v))
			{
				return false;
			}
		}
	}
	return true;
}

//determine si une abaille peut y voler
bool World::isFlyable(const Vec2d& position)
{
	//convertir coordonnées graphiques en grille	
	int x (toGrid(position.x));
	int y (toGrid(position.y));
	
	//a verifier si besoin de toUnidToric ou jute toUnid
	if (cells_[toUnidToric(x,y)]== Kind::rock)// ajout d'une fonction pour coordonnées toriques.
		{
			return false;
		}
	return true;
}

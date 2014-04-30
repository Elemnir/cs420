#include "particle.h"

#include <random>
#include <cmath>

double Particle::inertia, Particle::cognit, Particle::social;
double Particle::maxvel, Particle::width, Particle::height;
int Particle::probnum;

std::mt19937 Particle::rng(std::random_device{}());

Particle::Particle()
{//initialize the posistion and velocity
    std::uniform_real_distribution<double> wdist(-(width/2.0f),(width/2.0f));
    std::uniform_real_distribution<double> hdist(-(height/2.0f),(height/2.0f));
    
    xbest = xpos = wdist(rng);
    ybest = ypos = hdist(rng);
    xvel = yvel = 0.0f;
}

void Particle::update()
{//update the velocity, position, and performance
}

double quality(Particle const& p)
{//calculates the performance of a given Particle switches on probnum
    double mdist = std::pow(Particle::width,2) + std::pow(Particle::height,2);
    mdist = sqrt(mdist) / 2.0f;
    double pdist = std::sqrt(std::pow((p.xpos-20),2) + std::pow((p.ypos-7),2));
    double ndist = std::sqrt(std::pow((p.xpos+20),2) + std::pow((p.ypos+7),2));

    double rval = 0.0f;
    switch(Particle::probnum)
    {
        case 2:
            rval = 9 * std::fmax(0.0f, 10 - (pdist * pdist));
            rval += 10 * (1 - (pdist / mdist));
            rval += 70 * (1 - (ndist / mdist));
            break;
        case 1:
        default:
            rval = 100 * (1 - (pdist / mdist));
            break;
    }
    return rval;
}

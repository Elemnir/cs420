#include "particle.h"

#include <string>
#include <fstream>
#include <random>
#include <cmath>

std::vector<Particle> pop;
double Particle::xglobal, Particle::yglobal;
double Particle::inertia, Particle::cognit, Particle::social;
double Particle::maxvel = 10, Particle::width = 100, Particle::height = 100;
int Particle::probnum = 1;

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
    std::uniform_real_distribution<double> dist(0,1);
    
    //modify velocity
    xvel *= inertia;
    xvel += cognit * dist(rng) * (xbest - xpos);
    xvel += social * dist(rng) * (xglobal - xpos);

    yvel *= inertia;
    yvel += cognit * dist(rng) * (ybest - ypos);
    yvel += social * dist(rng) * (yglobal - ypos);
    
    if (std::fabs(xvel) > std::pow(maxvel, 2.0f))
        xvel *= maxvel / std::sqrt(std::pow(xvel, 2.0f) + std::pow(yvel, 2.0f));

    if (std::fabs(yvel) > std::pow(maxvel, 2.0f))
        yvel *= maxvel / std::sqrt(std::pow(xvel, 2.0f) + std::pow(yvel, 2.0f));

    //modify position
    xpos += xvel;
    ypos += yvel;

    //update personal best
    if (quality(xpos, ypos) > quality(xbest, ybest))
    {
        xbest = xpos;
        ybest = ypos;
    }

    //update global best
    if (quality(xpos, ypos) > quality(xglobal, yglobal))
    {
        xglobal = xpos;
        yglobal = ypos;
    }
}

double quality(double x, double y)
{//calculates the performance of a given Particle, switches on probnum
    double mdist = std::pow(Particle::width, 2.0f);
    mdist += std::pow(Particle::height, 2.0f);
    mdist = std::sqrt(mdist) / 2.0f;
    double pdist = std::sqrt(std::pow(x - 20, 2.0f) + std::pow(y - 7, 2.0f));
    double ndist = std::sqrt(std::pow(x + 20, 2.0f) + std::pow(y + 7, 2.0f));

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

void printImage(std::string const& fname)
{//prints out the population to a pgm file
    int x, y;
    std::vector<std::vector<int>> image(int(Particle::height), 
            std::vector<int>(int(Particle::width), 255));

    //construct the image as a vector of pixel hue values
    for (auto i : pop)
    {
        x = (Particle::width / 2) + std::floor(i.xpos);
        y = (Particle::height / 2) - std::floor(i.ypos);
        if (x < image.back().size() && x >= 0 && y < image.size() && y >= 0)
        {
            if (image[y][x] == 255)
                image[y][x] = 192;
            else if (image[y][x] != 0)
                image[y][x] -= 8;
        }
    }

    //write the image vector to a pgm file
    std::ofstream out((fname + ".pgm").c_str());
    out << "P2\n" << Particle::width << " " << Particle::height << "\n255\n";
    for (auto i : image)
    {
        for (auto j : i)
        {
            out << j << ' ';
        }
        out << '\n';
    }
}

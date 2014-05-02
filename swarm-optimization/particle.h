#ifndef PARTICLE_H
#define PARTICLE_H

#include <random>
#include <string>
#include <vector>

class Particle
{
  public:
    Particle();
    void update();

    double xpos, ypos; //particle position
    double xvel, yvel; //particle velocity
    double xbest, ybest; //personal best position
 
    static double xglobal, yglobal; //global best position
    static double inertia, cognit, social; //global system parameters
    static double maxvel, width, height; //world height and width
    static int probnum; //selects the problem to use

    static std::mt19937 rng; //global random number generator
};

double quality(double x, double y);
void printImage(std::string const& fname);

extern std::vector<Particle> pop;

#endif // PARTICLE_H

#include <iostream>
#include <string>
#include <vector>

#include "particle.h"

std::vector<Particle> pop;

int main(int argc, char** argv)
{
    //setup and read in commandline parameters
    int epoch_count = 20, population_size;
    switch (argc)
    {
        case 7:
            Particle::probnum = std::stoi(argv[6]);
        case 6:
            epoch_count = std::stoi(argv[1]);
            population_size = std::stoi(argv[2]);
            Particle::inertia = std::stod(argv[3]);
            Particle::cognit = std::stod(argv[4]);
            Particle::social = std::stod(argv[5]);
            break;

        default:
            return -1;
    }

    //average over the given number of epochs
    for (int epoch = 0; epoch < epoch_count; ++epoch)
    {
        //initialize the population
        pop.clear();
        pop.resize(population_size);

        //find the initial best particle
        double bestquality = 0.0f, curquality;
        for (auto i : pop)
        {
            curquality = quality(i.xpos, i.ypos);
            if (curquality > bestquality)
            {
                Particle::xglobal = i.xpos;
                Particle::yglobal = i.ypos;
                bestquality = curquality;
            }
        }

        //loop until within an error threshold
    }
}

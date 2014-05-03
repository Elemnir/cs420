#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "particle.h"

int main(int argc, char** argv)
{
    //setup and read in commandline parameters
    int epoch_limit;
    switch (argc)
    {
        case 7:
            Particle::probnum = std::stoi(argv[6]);
        case 6:
            epoch_limit = std::stoi(argv[1]);
            pop.resize(std::stoi(argv[2]));
            Particle::inertia = std::stod(argv[3]);
            Particle::cognit = std::stod(argv[4]);
            Particle::social = std::stod(argv[5]);
            break;

        default:
            return -1;
    }

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
    double xerr, yerr, threshold = 0.01;
    int epoch = 0;
    do {
        //update the population
        for (auto i : pop)
            i.update();

        //calculate error
        xerr = yerr = 0.0f;
        for (auto i : pop)
        {
            xerr += std::pow(i.xpos - Particle::xglobal, 2.0f);
            yerr += std::pow(i.ypos - Particle::yglobal, 2.0f);
        }
        xerr = std::sqrt((1.0f / (2 * pop.size())) * xerr);
        yerr = std::sqrt((1.0f / (2 * pop.size())) * yerr);

        printImage("frame" + std::to_string(epoch));

    } while (epoch++ < epoch_limit && (xerr > threshold || yerr > threshold));

    std::cout << "Convergence at epoch:" << epoch << std::endl;
    return 0;
}

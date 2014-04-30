#include "particle.h"

#include <random>

std::mt19937 Particle::rng(std::random_device{}());

Particle::Particle()
{
}

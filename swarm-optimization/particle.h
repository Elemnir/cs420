#include <random>

class Particle
{
  public:
    Particle();
  private:
    double xpos, ypos;
    double xvel, yvel;

    static std::mt19937 rng; //global random number generator
    static double inertia, cognition, social, maxvel;
};

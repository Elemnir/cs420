#include <random>

class Particle
{
  public:
    Particle();
    void update();

    double xpos, ypos; //particle position
    double xvel, yvel; //particle velocity
    double xbest, ybest; //personal best position
 
    static double inertia, cognit, social; //global system parameters
    static double maxvel, width, height; //world height and width
    static int probnum; //selects the problem to use

    static std::mt19937 rng; //global random number generator
};

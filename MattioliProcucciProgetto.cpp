#include <vector>
#include <random>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

struct Boid
{
    double x{0};
    double y{0};
    double vx{10};
    double vy{10};
};

class Flock
{
    private:
    std::vector<Boid> flock_{};

    public:
    void add(int n)
    {
        std::default_random_engine eng;
        std::uniform_real_distribution<double> position(0.,100.);
        std::uniform_real_distribution<double> speed(0.,100.);

        for (int i = 0; i < n; i++)
        {
            double rand_x{position(eng)};
            double rand_y{position(eng)};
            double rand_vx{speed(eng)};
            double rand_vy{speed(eng)};
            Boid boid{rand_x, rand_y, rand_vx, rand_vy};
            flock_.push_back(boid);
        }
    }
    
    int size()
    {
        return flock_.size();
    }
};

TEST_CASE("Testing the Flock class")
{
    Flock f{};
     
    SUBCASE("Testing the add function")
    {
        f.add(10);
        CHECK(f.size() == 10);
    }
}
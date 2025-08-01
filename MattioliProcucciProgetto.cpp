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
    //function adds n boids to the flock, their position and velocity are randomly generated
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

    void add(Boid b)
    //function that adds a single boid to the flock
    {
        flock_.push_back(b);
    }
    
    int size()
    {
        return flock_.size();
    }

    void update_position(int refresh_rate)
    //function that updates the position of all boids after: refresh_rate seconds 
    {
        for (auto it = flock_.begin(), last = flock_.end(); it < last; it++)
        {
            (*it).x += refresh_rate * (*it).vx;
            (*it).y += refresh_rate * (*it).vy;
        }
        
    }

    double get_positionx(int i)
    //function returns the x coordinate of the i th oid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).x;
        }
        else{return -1.;}
    }
    double get_positiony(int i)
    //function returns the x coordinate of the i th oid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).y;
        }
        else{return -1.;}
    }
};

TEST_CASE("Testing the Flock class")
{
    Flock f{};
     
    SUBCASE("Testing the add function")
    {
        f.add(10);
        CHECK(f.size() == 10);
        Boid b{0,0,2,2};
        f.add(b);
        CHECK(f.size() == 11);
    }

    SUBCASE("testing the update_position function")
    {
        Boid b{0,0,2,2};
        Boid b1{1,1,3,4};
        f.add(b);
        f.add(b1);
        f.update_position(1);
        CHECK(f.get_positionx(0) == 2.);
        CHECK(f.get_positiony(0) == 2.);
        CHECK(f.get_positionx(1) == 4.);
        CHECK(f.get_positiony(1) == 5.);
        CHECK(f.get_positionx(3) == -1.);
    }
}
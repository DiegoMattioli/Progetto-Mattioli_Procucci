#include <vector>
#include <random>
#include <cmath>
#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

struct Boid
{
    double x{0.};
    double y{0.};
    double vx{1.0};
    double vy{1.0};
    std::vector<int> nearby{};
    double v_sepx{0.};
    double v_sepy{0.};
    double v_aligx{0.};
    double v_aligy{0.};
    double v_cohex{0.};
    double v_cohey{0.};
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

        for (int i = 0; i < n; ++i)
        {
            double rand_x{position(eng)};
            double rand_y{position(eng)};
            double rand_vx{speed(eng)};
            double rand_vy{speed(eng)};
            Boid boid{rand_x, rand_y, rand_vx, rand_vy};
            flock_.push_back(boid);
        }
    }

    void add(Boid const& b)
    //function that adds a single boid to the flock
    {
        flock_.push_back(b);
    }
 
    void update_position(int refresh_rate)
    //function that updates the position of all boids after: refresh_rate seconds 
    {
        for (auto it = flock_.begin(); it < flock_.end(); ++it)
        {
            (*it).vx += ((*it).v_sepx + (*it).v_aligx + (*it).v_cohex);
            (*it).x += refresh_rate * (*it).vx;
            (*it).v_sepx = 0.;
            (*it).v_aligx = 0.;
            (*it).v_cohex = 0.;
            if ((*it).x < 0.)
            {
                (*it).x += (100. * std::floor((*it).y / 100.));
            }
            else if ((*it).x > 100.)
            {
                (*it).x += -(100. * std::floor((*it).x / 100.));
            }
            
            (*it).vy += ((*it).v_sepy + (*it).v_aligy + (*it).v_cohey);
            (*it).y += refresh_rate * (*it).vy;
            (*it).v_sepy = 0.;
            (*it).v_aligy = 0.;
            (*it).v_cohey = 0.;
            if ((*it).y < 0.)
            {
                (*it).y += (100. * std::floor((*it).y / 100.));
            }
            else if ((*it).y > 100.)
            {
                (*it).y += -(100. * std::floor((*it).y / 100.));
            }
        }
    }

    void update_velocity(double d, double ds, double s, double a, double c)
    //function that updates the vector representing the velocity of each boid)
    {
        for (auto it = flock_.begin(); it < flock_.end(); ++it)
        {
            double v_separationx{0};
            double v_separationy{0};
            double v_alignementx{0};
            double v_alignementy{0};
            double x_cm{0};
            double y_cm{0};
            double v_cohesionx{0};
            double v_cohesiony{0};
            (*it).nearby = {};

            for (auto itt = flock_.begin(); itt < flock_.end(); ++itt)
            {
                double distance{std::sqrt(std::pow(((*it).x - (*itt).x), 2) + std::pow(((*it).y - (*itt).y), 2))};
                if (distance < d && it != itt)
                {
                    (*it).nearby.push_back(itt - flock_.begin());

                    x_cm += (*itt).x;
                    y_cm += (*itt).y;

                    if (distance < ds)
                    {
                        v_separationx += -((*itt).x - (*it).x);
                        v_separationy += -((*itt).y - (*it).y);
                    }
                }
                
                v_alignementx += ((*itt).vx - (*it).vx);
                v_alignementy += ((*itt).vy - (*it).vy);
            }
            v_cohesionx += (x_cm / ((*it).nearby.size())) - (*it).x;
            v_cohesiony += (y_cm / ((*it).nearby.size())) - (*it).y;
            (*it).v_sepx += v_separationx *s ;
            (*it).v_sepy += v_separationx *s ;
            (*it).v_aligx += v_alignementx *(a/(flock_.size()-1));
            (*it).v_aligy += v_alignementy *(a/(flock_.size()-1));
            (*it).v_cohex += v_cohesionx *c;
            (*it).v_cohey += v_cohesiony *c;
        }
        
    }
       
    int size()
    {
        return flock_.size();
    }


    double get_positionx(int i)
    //function returns the x coordinate of the i th boid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).x;
        }
        else{return -1.;}
    }
    double get_positiony(int i)
    //function returns the y coordinate of the i th boid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).y;
        }
        else{return -1.;}
    }

    double get_vx(int i)
    //function returns the x coordinate of the velocity of the i th boid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).vx;
        }
        else{return 0.;}
    }
    double get_vy(int i)
    //function returns the y coordinate of the velocity of the i th boid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).vy;
        }
        else{return 0.;}
    }

    int get_nearby_size(int i)
    //function that returns the amount of boids close to the i th boid
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).nearby.size();
        }
        else{return -1;}
    }
};

TEST_CASE("Testing the Flock class")
{
    Flock f{};
    Boid b{0,0,2,2};
    Boid b1{1,1,3,4};
    f.add(b);
    f.add(b1);
     
    SUBCASE("Testing the add function")
    {
        f.add(10);
        CHECK(f.size() == 12);
    }

    SUBCASE("testing the update_position function")
    {
        f.update_position(1);
        CHECK(f.get_positionx(0) == 2.);
        CHECK(f.get_positiony(0) == 2.);
        CHECK(f.get_positionx(1) == 4.);
        CHECK(f.get_positiony(1) == 5.);
        CHECK(f.get_positionx(3) == -1.);

        f.update_position(50);
        CHECK(f.get_positionx(0) == 2.);
        CHECK(f.get_positiony(0) == 2.);
        CHECK(f.get_positionx(1) == 54.);
        CHECK(f.get_positiony(1) == 5.);
    }

    SUBCASE("testing the update_velocity function")
    {
        SUBCASE("testing the correct updating of the 'nearby' vector of each boid")
        {
            f.update_velocity(2., 1., 0.5, 0.1, 0.2);
            CHECK(f.get_nearby_size(0) == 1);
            CHECK(f.get_nearby_size(1) == 1);
            CHECK(f.get_nearby_size(2) == -1);
            f.update_position(4);
            f.update_velocity(2., 1., 0.5, 0.1, 0.2);
            CHECK(f.get_nearby_size(0) == 0);
            CHECK(f.get_nearby_size(1) == 0);
        }

        SUBCASE("testing the correct updating of the boids' velocities (two boids)")
        {
            f.update_velocity(3., 2., 0.5, 0.1, 0.2);
            f.update_position(1);
            CHECK(f.get_vx(0) == doctest::Approx(1.8));
            CHECK(f.get_vy(0) == doctest::Approx(1.9));
            CHECK(f.get_vx(1) == doctest::Approx(3.2));
            CHECK(f.get_vy(1) == doctest::Approx(4.1));
            CHECK(f.get_vx(2) == 0.);
        }

        SUBCASE("testing the correct updating of the boids' velocities (three boids)")
        {
            Boid b2{2., 2., 1., 1.};
            f.add(b2);
            f.update_velocity(3., 2., 0.5, 0.5, 0.2);
            f.update_position(1);
            CHECK(f.get_vx(0) == doctest::Approx(1.8));
            CHECK(f.get_vy(0) == doctest::Approx(2.05));
            CHECK(f.get_vx(1) == doctest::Approx(2.25));
            CHECK(f.get_vy(1) == doctest::Approx(2.75));
            CHECK(f.get_vx(2) == doctest::Approx(1.95));
            CHECK(f.get_vy(2) == doctest::Approx(2.2));
        }
    }
}
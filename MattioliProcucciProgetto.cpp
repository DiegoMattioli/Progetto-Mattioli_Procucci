#include <vector>
#include <random>
#include <cmath>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

struct Boid
{
    double x{0};
    double y{0};
    double vx{10};
    double vy{10};
    std::vector<int> nearby{};
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

    void add(const Boid& b)
    //function that adds a single boid to the flock
    {
        flock_.push_back(b);
    }
 
    void update_position(int refresh_rate)
    //function that updates the position of all boids after: refresh_rate seconds 
    {
        for (auto it = flock_.begin(); it < flock_.end(); ++it)
        {
            (*it).x += refresh_rate * (*it).vx;
            (*it).y += refresh_rate * (*it).vy;
        }
        
    }

    void update_velocity(double d, double ds, double s, double a, double c)
    //function that updates the vector representing the velocity of each boid)
    {
        for (auto it = flock_.begin(); it < flock_.end(); ++it)
        {
            double v_separazionex{0};
            double v_separazioney{0};
            double v_allineamentox{0};
            double v_allineamentoy{0};
            double x_cm{0};
            double y_cm{0};
            double v_coesionex{0};
            double v_coesioney{0};
            (*it).nearby = {};

            for (auto itt = flock_.begin(); itt < flock_.end(); ++itt)
            {
                double distance{std::sqrt(std::pow(((*it).x - (*itt).x), 2) + std::pow(((*it).y - (*itt).y), 2))};
                if (distance < d && distance != 0)
                {
                    (*it).nearby.push_back(itt - flock_.begin());

                    x_cm += (*itt).x / (flock_.size()-1);
                    y_cm += (*itt).y / (flock_.size()-1);

                    if (distance < ds)

                    {
                        v_separazionex += -(s * ((*itt).x - (*it).x));
                        v_separazioney += -(s * ((*itt).y - (*it).y));
                    }
                }
                
                v_allineamentox += (a/(flock_.size()-1)) * ((*itt).vx - (*it).vx);
                v_allineamentoy += (a/(flock_.size()-1)) * ((*itt).vy - (*it).vy);
            }
            v_coesionex += c * (x_cm - (*it).x);
            v_coesioney += c * (y_cm - (*it).y);
            (*it).vx += (v_allineamentox + v_separazionex + v_coesionex);
            (*it).vy += (v_allineamentoy + v_separazioney + v_coesioney);
        }
        
    }
       
    int size()
    {
        return flock_.size();
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
    }

    SUBCASE("testing the update_velocity function")
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
}
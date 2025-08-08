#include <vector>
#include <random>
#include <cmath>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

class Boid
{
    private:
    std::vector<int> nearby_{};
    double vsepx_{0.};
    double vsepy_{0.};
    double valigx_{0.};
    double valigy_{0.};
    double vcohex_{0.};
    double vcohey_{0.};
    double x_;
    double y_;
    double vx_;
    double vy_;
    
    public:
    Boid(double x, double y, double vx, double vy): x_{x}, y_{y}, vx_{vx}, vy_{vy} {}
    double x()const{return x_;} //returns the x coordinate of the position of the boid
    double y()const{return y_;} //returns the y coordinate of the position of the boid
    double vx()const{return vx_;} //returns the x coordinate of the velocity of the boid
    double vy()const{return vy_;} //returns the y coordinate of the velocity of the boid
    void add_x(double const& add){x_ += add;} //modifies the x coordinate of the position of the boid
    void add_y(double const& add){y_ += add;} //modifies the y coordinate of the position of the boid
    void add_vx(double const& add){vx_ += add;} //modifies the x coordinate of the velocity of the boid
    void add_vy(double const& add){vy_ += add;} //modifies the y coordinate of the velocity of the boid

    void reset()
    {
        nearby_ = {};
        vsepx_ = 0.;
        vsepy_ = 0.;
        valigx_ = 0.;
        valigy_ = 0.;
        vcohex_ = 0.;
        vcohey_ = 0.;
    }

    void add_vsepx(double const& add){vsepx_ += add;}
    void add_vsepy(double const& add){vsepy_ += add;}
    void add_valigx(double const& add){valigx_ += add;}
    void add_valigy(double const& add){valigy_ += add;}
    void add_vcohex(double const& add){vcohex_ += add;}
    void add_vcohey(double const& add){vcohey_ += add;}
    void mult_vsep(double const& mult){
        vsepx_ *= mult;
        vsepy_ *= mult;}
    void mult_valig(double const& mult){
        valigx_ *= mult;
        valigy_ *= mult;}
    void mult_vcohe(double const& mult){
        vcohex_ *= mult;
        vcohey_ *= mult;
    }
    double get_sumvx()const{return (valigx_ + vsepx_ +vcohex_);}
    double get_sumvy()const{return (valigy_ + vsepy_ +vcohey_);}
    int get_nearbysize()const{return nearby_.size();}
    void add_nearby(int index){nearby_.push_back(index);}
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
            (*it).add_vx((*it).get_sumvx());
            (*it).add_x(refresh_rate * (*it).vx());
            if ((*it).x() < 0.)
            {
                (*it).add_x(100. * std::floor((*it).x() / 100.));
            }
            else if ((*it).x() > 100.)
            {
                (*it).add_x(-100. * std::floor((*it).x() / 100.));
            }
            
            (*it).add_vy((*it).get_sumvy());
            (*it).add_y(refresh_rate * (*it).vy());
            (*it).reset();
            if ((*it).y() < 0.)
            {
                (*it).add_y(100. * std::floor((*it).y() / 100.));
            }
            else if ((*it).y() > 100.)
            {
                (*it).add_y(-100. * std::floor((*it).y() / 100.));
            }
        }
    }

    void update_velocity(double const& d, double const& ds, double const& s, double const& a, double const& c)
    //function that updates the vector representing the velocity of each boid)
    {
        for (auto it = flock_.begin(); it < flock_.end(); ++it)
        {
            double x_cm{0};
            double y_cm{0};

            for (auto itt = flock_.begin(); itt < flock_.end(); ++itt)
            {
                double distance{std::sqrt(std::pow(((*it).x() - (*itt).x()), 2) + std::pow(((*it).y() - (*itt).y()), 2))};
                if (distance < d && it != itt)
                {
                    (*it).add_nearby(itt - flock_.begin());

                    x_cm += (*itt).x();
                    y_cm += (*itt).y();

                    if (distance < ds)
                    {
                        (*it).add_vsepx(-((*itt).x() - (*it).x()));
                        (*it).add_vsepy(-((*itt).y() - (*it).y()));
                    }
                }
                
                (*it).add_valigx((*itt).vx() - (*it).vx());
                (*it).add_valigy((*itt).vy() - (*it).vy());
            }
            (*it).add_vcohex((x_cm / ((*it).get_nearbysize())) - (*it).x());
            (*it).add_vcohey((y_cm / ((*it).get_nearbysize())) - (*it).y());
            (*it).mult_vsep(s);
            (*it).mult_valig(a/(flock_.size()-1));
            (*it).mult_vcohe(c);
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
            return (*it).x();
        }
        else{return -1.;}
    }
    double get_positiony(int i)
    //function returns the y coordinate of the i th boid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).y();
        }
        else{return -1.;}
    }

    double get_vx(int i)
    //function returns the x coordinate of the velocity of the i th boid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).vx();
        }
        else{return 0.;}
    }
    double get_vy(int i)
    //function returns the y coordinate of the velocity of the i th boid in the flock
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).vy();
        }
        else{return 0.;}
    }

    int get_nearby_size(int i)
    //function that returns the amount of boids close to the i th boid
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).get_nearbysize();
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

        Boid b2{3., 3., 2.1, 1.4};
        f.add(b2);
        f.update_position(3);
        CHECK(f.get_positionx(2) == doctest::Approx(9.3));
        CHECK(f.get_positiony(2) == doctest::Approx(7.2));
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
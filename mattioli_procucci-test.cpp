#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "mattioli_procucci0.hpp"
#include "doctest.h"
#include <iostream>
#include <cmath>

TEST_CASE("Testing the Flock class")
{
    mp::Parameters p{3., 2., 0.5, 0.1, 0.2};
    mp::Flock f{p};
    mp::Boid b{0,0,2,2};
    mp::Boid b1{1,1,3,4};
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
        CHECK_THROWS(f.get_positionx(3));

        f.update_position(50);
        CHECK(f.get_positionx(0) == 102.);
        CHECK(f.get_positiony(0) == 102.);
        CHECK(f.get_positionx(1) == 154.);
        CHECK(f.get_positiony(1) == 5.);

        mp::Boid b2{3., 3., 2.1, 1.4};
        f.add(b2);
        f.update_position(3);
        CHECK(f.get_positionx(2) == doctest::Approx(9.3));
        CHECK(f.get_positiony(2) == doctest::Approx(7.2));
    }

    SUBCASE("testing the update_velocity function")
    {
        SUBCASE("testing the correct updating of the 'nearby' vector of each boid")
        {
            f.update_velocity();
            CHECK(f.get_nearby_size(0) == 1);
            CHECK(f.get_nearby_size(1) == 1);
            CHECK_THROWS(f.get_nearby_size(2));
            f.update_position(7);
            f.update_velocity();
            CHECK(f.get_nearby_size(0) == 0);
            CHECK(f.get_nearby_size(1) == 0);
        }

        SUBCASE("testing the correct updating of the boids' velocities (two boids)")
        {
            f.update_velocity();
            f.update_position(1);
            CHECK(f.get_vx(0) == doctest::Approx(1.8));
            CHECK(f.get_vy(0) == doctest::Approx(1.9));
            CHECK(f.get_vx(1) == doctest::Approx(3.2));
            CHECK(f.get_vy(1) == doctest::Approx(4.1));
            CHECK_THROWS(f.get_vx(2));
        }

        SUBCASE("testing the correct updating of the boids' velocities (three boids)")
        {
            mp::Boid b2{2., 2., 1., 1.};
            f.add(b2);
            f.update_velocity();
            f.update_position(1);
            CHECK(f.get_vx(0) == doctest::Approx(1.8));
            CHECK(f.get_vy(0) == doctest::Approx(1.85));
            CHECK(f.get_vx(1) == doctest::Approx(2.85));
            CHECK(f.get_vy(1) == doctest::Approx(3.75));
            CHECK(f.get_vx(2) == doctest::Approx(1.35));
            CHECK(f.get_vy(2) == doctest::Approx(1.4));
        }
    }
}

TEST_CASE("Testing flock simulation all together")
{
    mp::Parameters p{3., 2., 0.5, 0.5, 0.5};
    mp::Flock f{p};
    f.add(11);
    REQUIRE(f.size() == 11);

    for (int i = 0; i < 20; ++i)
    {
        f.update_velocity();
        f.update_position(1);
        for (int j = 0; j < 11; ++j)
        {
            CHECK(std::isnan(f.get_positionx(j)) == false);
            CHECK(std::isnan(f.get_positiony(j)) == false);
            CHECK(std::isnan(f.get_vx(j)) == false);
            CHECK(std::isnan(f.get_vy(j)) == false);
        }
    }
    
}
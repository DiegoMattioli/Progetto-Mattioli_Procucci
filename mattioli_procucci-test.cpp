#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "mattioli_procucci0.hpp"
#include "doctest.h"
#include <iostream>
#include <cmath>

TEST_CASE("Testing the Flock class")
{
    mp::Parameters p{90., 10., 0.5, 0.5, 0.5};
    mp::Flock f{p};
    CHECK_THROWS(f.update_position(1));
    CHECK_THROWS(f.update_velocity());
    mp::Boid b{0., 0., 2., 2.};
    mp::Boid b1{1., 1., 3., 4.};
    f.add(b);
    CHECK_THROWS(f.update_position(1));
    CHECK_THROWS(f.update_velocity());
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

        f.update_position(100);
        CHECK(f.get_positionx(0) == 2.);
        CHECK(f.get_positiony(0) == 2.);
        CHECK(f.get_positionx(1) == 104.);
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
            mp::Boid b2{100., 100, -1., -1};
            f.add(b2);
            f.update_velocity();
            CHECK(f.get_nearby_size(0) == 1);
            CHECK(f.get_nearby_size(1) == 1);
            CHECK(f.get_nearby_size(2) == 0);
            CHECK_THROWS(f.get_nearby_size(3));
            f.update_position(10);
            f.update_velocity();
            CHECK(f.get_nearby_size(0) == 2);
            CHECK(f.get_nearby_size(1) == 2);
        }

        SUBCASE("testing the correct updating of the boids' velocities (two boids)")
        {
            f.update_velocity();
            f.update_position(1);
            CHECK(f.get_vx(0) == doctest::Approx(2.5));
            CHECK(f.get_vy(0) == doctest::Approx(3.));
            CHECK(f.get_vx(1) == doctest::Approx(2.5));
            CHECK(f.get_vy(1) == doctest::Approx(3.));
            CHECK_THROWS(f.get_vx(2));
        }

        SUBCASE("testing the correct updating of the boids' velocities (three boids)")
        {
            mp::Boid b2{2., 2., 1., 1.};
            f.add(b2);
            f.update_velocity();
            f.update_position(1);
            CHECK(f.get_vx(0) == doctest::Approx(1.25));
            CHECK(f.get_vy(0) == doctest::Approx(1.5));
            CHECK(f.get_vx(1) == doctest::Approx(2.25));
            CHECK(f.get_vy(1) == doctest::Approx(2.75));
            CHECK(f.get_vx(2) == doctest::Approx(2.5));
            CHECK(f.get_vy(2) == doctest::Approx(2.75));
        }
        SUBCASE ("testing the correct updating of the boids' velocities (five boids)")
        {
            mp::Boid b2{2., 2., 1., 0.};
            mp::Boid b3{3., 1., 0., 2.};
            mp::Boid b4{4., 2., 2., 0.};
            f.add(b2); f.add(b3); f.add(b4);
            f.update_velocity();
            f.update_position(1);
            CHECK(f.get_vx(0) == doctest::Approx(-2));
            CHECK(f.get_vy(0) == doctest::Approx(-0.5));
            CHECK(f.get_vx(1) == doctest::Approx(0.25));
            CHECK(f.get_vy(1) == doctest::Approx(2.125));
            CHECK(f.get_vx(2) == doctest::Approx(1.375));
            CHECK(f.get_vy(2) == doctest::Approx(2.5));
            CHECK(f.get_vx(3) == doctest::Approx(2.875));
            CHECK(f.get_vy(3) == doctest::Approx(1.375));
            CHECK(f.get_vx(4) == doctest::Approx(5.5));
            CHECK(f.get_vy(4) == doctest::Approx(2.5));

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
            CHECK(f.get_positionx(j) <= 200.);
            CHECK(f.get_positiony(j) <= 200.);
        }
    }
    
}
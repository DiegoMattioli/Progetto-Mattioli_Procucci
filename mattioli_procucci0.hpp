#ifndef MP_MATTIOLIPROCUCCI_HPP
#define MP_MATTIOLIPROCUCCI_HPP

#include <vector>

namespace mp{

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
        double x()const; //returns the x coordinate of the position of the boid
        double y()const; //returns the y coordinate of the position of the boid
        double vx()const; //returns the x coordinate of the velocity of the boid
        double vy()const; //returns the y coordinate of the velocity of the boid
        void add_x(double const& add); //modifies the x coordinate of the position of the boid
        void add_y(double const& add); //modifies the y coordinate of the position of the boid
        void add_vx(double const& add); //modifies the x coordinate of the velocity of the boid
        void add_vy(double const& add); //modifies the y coordinate of the velocity of the boid

        void reset();

        void add_vsepx(double const& add);
        void add_vsepy(double const& add);
        void add_valigx(double const& add);
        void add_valigy(double const& add);
        void add_vcohex(double const& add);
        void add_vcohey(double const& add);
        void mult_vsep(double const& mult);
        void mult_valig(double const& mult);
        void mult_vcohe(double const& mult);

        double get_sumvx()const;
        double get_sumvy()const;
        int get_nearbysize()const;
        void add_nearby(int index);
    };

    class Flock
    {
        private:
        std::vector<Boid> flock_{};

        public:
        void add(int n); //function adds n boids to the flock, their position and velocity are randomly generated
    

        void add(Boid const& b); //function that adds a single boid to the flock
 
        void update_position(int refresh_rate);
        //function that updates the position of all boids after: refresh_rate seconds 

        void update_velocity(double const& d, double const& ds, double const& s, double const& a, double const& c);
        //function that updates the vector representing the velocity of each boid)
       
        int size();

        double get_positionx(int i);
        //function returns the x coordinate of the i th boid in the flock
    
        double get_positiony(int i);
        //function returns the y coordinate of the i th boid in the flock

        double get_vx(int i);
        //function returns the x coordinate of the velocity of the i th boid in the flock

        double get_vy(int i);
        //function returns the y coordinate of the velocity of the i th boid in the flock

        int get_nearby_size(int i);
        //function that returns the amount of boids close to the i th boid
    };
};

#endif
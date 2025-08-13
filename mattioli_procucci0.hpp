#ifndef MP_MATTIOLIPROCUCCI_HPP
#define MP_MATTIOLIPROCUCCI_HPP

#include <vector>

namespace mp{

    struct Parameters
    {
        double distance;
        //parameter that defines when two or more boids can be considered close to each other
        //if the distance between two or more boids is smaller than this parameter, they are considered 'close to each other'
        double separation_distance;
        //parameter that defines the minimum distance boids should keep from each other, if the distance between two
        //or more boids is smaller than this parameter they will make try to distance themselves in order to avoid collisions
        double separation;
        //parameter that determines how strongly boids diverge from each other, high values of 's' will result in a flock
        //whose boids are further apart from each other
        double alignement;
        //parameter that determines the alignement of the blods in the flock, high values of 'a' will result in a flock
        //whose boids follow a specific direction uniformly
        double cohesion;
        //parameter that determines the cohesione between the boids of the flock, high values of 'c' will result in a flock
        //whose boids don't diverge form the group 

    };
    

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
        double d_;
        double ds_;
        double s_;
        double a_;
        double c_;
        //parameters fo the flock (defined in the struct mp::Parameters)

        double mean_vx_;
        double mean_vy_;
        //coordinates of the mean velocity of the boids in the flock

        public:
        Flock(Parameters const& p): d_{p.distance}, ds_{p.separation_distance}, s_{p.separation}, a_{p.alignement}, c_{p.cohesion} {}
        void add(int n); //function adds n boids to the flock, their position and velocity are randomly generated
    

        void add(Boid const& b); //function that adds a single boid to the flock
 
        void update_position(int refresh_rate);
        //function that updates the position of all boids after: refresh_rate seconds 

        void update_velocity();
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

        double get_mean_velocity();
        //functiom that returns the mean velocity of the boids in the flock
    };
};

#endif
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
        std::vector<int> nearby_{}; //index of nearby boids
        double vsepx_{0.};
        double vsepy_{0.};
        //modification to the boid's velocity after applying the rule of separation

        double valigx_{0.};
        double valigy_{0.};
        //modification to the boid's velocity after applying the rule of alignement

        double vcohex_{0.};
        double vcohey_{0.};
        //modification to the boid's velocity after applying the rule of cohesion

        double x_;
        double y_;
        double vx_;
        double vy_;
        //boid's coordinates
    
        public:
        Boid(double x, double y, double vx, double vy): x_{x}, y_{y}, vx_{vx}, vy_{vy} {}
        double x()const;
        double y()const;
        //return the  coordinates of the boid's position
        double vx()const;
        double vy()const;
        //return the coordinates of the boid's velocity

        void add_x(double add);
        void add_y(double add);
        //modifies the coordinates of the boid's position
        void add_vx(double add);
        void add_vy(double add);
        //modifies the coordinates of the boid's velocity

        void reset();
        //resets to 0 the values of vsepx_, vsepy_, valigx_, valigy_, vcohex_, vcohey_

        void add_vsepx(double add);
        void add_vsepy(double add);
        //modifies vsepx_, vsepy_ (addition)
        void add_valigx(double add);
        void add_valigy(double add);
        //modifies valig_, valigy_ (addition)
        void add_vcohex(double add);
        void add_vcohey(double add);
        //modifies vcohex_, vcohey_ (addition)

        void mult_vsep(double mult); //modifies vsepx_, vsepy_ (multiplication)
        void mult_valig(double mult); //modifies valig_, valigy_ (multiplication)
        void mult_vcohe(double mult); //modifies vcohex_, vcohey_ (multiplication)

        void half_v();

        double get_sumvx()const; //adds toghether vsepx_, valigx_, vcohex_ and returns the sum
        double get_sumvy()const; //adds toghether vsepy_, valigy_, vcohey_ and returns the sum
        int get_nearbysize()const; //returns the amount of boids close to this particular boid
        void add_nearby(int index);
        //adds an index to the vector containing the indexes of the boids that are considered 'close' to this particular boid
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

        double mean_v_;
        double stddeviation_v_ {};
        //coordinates of the mean velocity and its standard deviation of the boids in the flock

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
        double get_stddeviation();
        //functiom that returns the standard deviation of the mean velocity of the boids in the flock
    };
};

#endif
#include "mattioli_procucci0.hpp"
#include <random>
#include <cmath>
#include <stdexcept>

namespace mp
{
    double mp::Boid::x()const{return x_;}
    double mp::Boid::y()const{return y_;}
    double mp::Boid::vx()const{return vx_;}
    double mp::Boid::vy()const{return vy_;}
    void mp::Boid::add_x(double const& add){{x_ += add;}}
    void mp::Boid::add_y(double const& add){{y_ += add;}}
    void mp::Boid::add_vx(double const& add){{vx_ += add;}}
    void mp::Boid::add_vy(double const& add){{vy_ += add;}}

    void mp::Boid::reset()
    {
        nearby_ = {};
        vsepx_ = 0.;
        vsepy_ = 0.;
        valigx_ = 0.;
        valigy_ = 0.;
        vcohex_ = 0.;
        vcohey_ = 0.;
    }

    void mp::Boid::add_vsepx(double const& add){vsepx_ += add;}
    void mp::Boid::add_vsepy(double const& add){vsepy_ += add;}
    void mp::Boid::add_valigx(double const& add){valigx_ += add;}
    void mp::Boid::add_valigy(double const& add){valigy_ += add;}
    void mp::Boid::add_vcohex(double const& add){vcohex_ += add;}
    void mp::Boid::add_vcohey(double const& add){vcohey_ += add;}

    void mp::Boid::mult_vsep(double const& multiply){
        vsepx_ *= multiply;
        vsepy_ *= multiply;}
    void mp::Boid::mult_valig(double const& multiply){
        valigx_ *= multiply;
        valigy_ *= multiply;}
    void mp::Boid::mult_vcohe(double const& multiply){
        vcohex_ *= multiply;
        vcohey_ *= multiply;
    }

    double mp::Boid::get_sumvx()const{return (valigx_ + vsepx_ +vcohex_);}
    double mp::Boid::get_sumvy()const{return (valigy_ + vsepy_ +vcohey_);}
    int mp::Boid::get_nearbysize()const{return static_cast<int>(nearby_.size());}
    void mp::Boid::add_nearby(int index){nearby_.push_back(index);}

    void mp::Flock::add(int n)
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

    void mp::Flock::add(Boid const& b)
    {
        flock_.push_back(b);
    }

    void mp::Flock::update_position(int refresh_rate)
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

    void mp::Flock::update_velocity(double const& d, double const& ds, double const& s, double const& a, double const& c)
    {
        for (auto it = flock_.begin(); it < flock_.end(); ++it)
        {
            (*it).reset();
            double x_cm{0};
            double y_cm{0};

            for (auto itt = flock_.begin(); itt < flock_.end(); ++itt)
            {
                double distance{std::sqrt(std::pow(((*it).x() - (*itt).x()), 2) + std::pow(((*it).y() - (*itt).y()), 2))};
                if (distance < d && it != itt)
                {
                    (*it).add_nearby(static_cast<int>(itt - flock_.begin()));

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

            if ((*it).get_nearbysize() != 0)
            {
                (*it).add_vcohex((x_cm / ((*it).get_nearbysize())) - (*it).x());
                (*it).add_vcohey((y_cm / ((*it).get_nearbysize())) - (*it).y());
            }
            (*it).mult_vsep(s);
            (*it).mult_valig(a/(static_cast<double>(flock_.size())-1.));
            (*it).mult_vcohe(c);
        }
    }

    int mp::Flock::size(){return static_cast<int>(flock_.size());}

    double mp::Flock::get_positionx(int i)
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).x();
        }
        else{throw std::runtime_error{"index out of range"};}
    }

    double mp::Flock::get_positiony(int i)
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).y();
        }
        else{throw std::runtime_error{"index out of range"};}
    }

    double mp::Flock::get_vx(int i)
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).vx();
        }
        else{throw std::runtime_error{"index out of range"};}
    }
    double mp::Flock::get_vy(int i)
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).vy();
        }
        else{throw std::runtime_error{"index out of range"};}
    }

    int mp::Flock::get_nearby_size(int i)
    {
        auto it = (flock_.begin() + i);
        if (it < flock_.end())
        {
            return (*it).get_nearbysize();
        }
        else{throw std::runtime_error{"index out of range"};}
    }
    
}
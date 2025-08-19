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

    void mp::Boid::add_vsepx(double add){vsepx_ += add;}
    void mp::Boid::add_vsepy(double add){vsepy_ += add;}
    void mp::Boid::add_valigx(double add){valigx_ += add;}
    void mp::Boid::add_valigy(double add){valigy_ += add;}
    void mp::Boid::add_vcohex(double add){vcohex_ += add;}
    void mp::Boid::add_vcohey(double add){vcohey_ += add;}

    void mp::Boid::mult_vsep(double multiply){
        vsepx_ *= multiply;
        vsepy_ *= multiply;}
    void mp::Boid::mult_valig(double multiply){
        valigx_ *= multiply;
        valigy_ *= multiply;}
    void mp::Boid::mult_vcohe(double multiply){
        vcohex_ *= multiply;
        vcohey_ *= multiply;
    }

    void mp::Boid::half_v()
    {
        vx_ *= 0.5;
        vy_ *= 0.5;
    }

    double mp::Boid::get_sumvx()const{return (valigx_ + vsepx_ +vcohex_);}
    double mp::Boid::get_sumvy()const{return (valigy_ + vsepy_ +vcohey_);}
    int mp::Boid::get_nearbysize()const{return static_cast<int>(nearby_.size());}
    void mp::Boid::add_nearby(int index){nearby_.push_back(index);}

    void mp::Flock::add(int n)
    {
        std::default_random_engine eng;
        std::uniform_real_distribution<double> position(0., 200.);
        std::uniform_real_distribution<double> speed(0.,10.);

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
        mean_vx_ = 0.;
        mean_vy_ = 0.;
        if (static_cast<int>(flock_.size()) > 0)
        {
            for (auto it = flock_.begin(); it < flock_.end(); ++it)
            {
                double vtot{std::hypot((*it).vx(), (*it).vy())};
                (*it).add_vx((*it).get_sumvx());
                mean_vx_ += (*it).vx() / static_cast<double>(flock_.size());
                (*it).add_x(refresh_rate * (*it).vx());
                if ((*it).x() < 0.)
                {
                    (*it).add_x(200. * std::floor((*it).x() / 200.));
                }
                else if ((*it).x() > 200.)
                {
                    (*it).add_x(-200. * std::floor((*it).x() / 200.));
                }
            
                (*it).add_vy((*it).get_sumvy());
                mean_vy_ += (*it).vy() / static_cast<double>(flock_.size());
                (*it).add_y(refresh_rate * (*it).vy());
                (*it).reset();
                if ((*it).y() < 0.)
                {
                    (*it).add_y(200. * std::floor((*it).y() / 200.));
                }
                else if ((*it).y() > 200.)
                {   
                    (*it).add_y(-200. * std::floor((*it).y() / 200.));
                }

                if (vtot > 30.){(*it).half_v();}
                
            }
        }
        else{throw std::runtime_error{"The flock must contain at least one boid"};}
    }

    void mp::Flock::update_velocity()
    {
        if (flock_.size() > 1)
        {
            for (auto it = flock_.begin(); it < flock_.end(); ++it)
            {
                (*it).reset();
                double x_cm{0.};
                double y_cm{0.};
                    
                for (auto itt = flock_.begin(); itt < flock_.end(); ++itt)
                {
                    double distance{std::hypot(((*it).x() - (*itt).x()), (*it).y() - (*itt).y())};
                    if (distance < d_ && it != itt)
                    {
                        
                        (*it).add_valigx((*itt).vx() - (*it).vx());
                        (*it).add_valigy((*itt).vy() - (*it).vy());

                        (*it).add_nearby(static_cast<int>(itt - flock_.begin()));
                                
                        x_cm += (*itt).x();
                        y_cm += (*itt).y();
                                
                        if (distance < ds_)
                        {
                            (*it).add_vsepx(-((*itt).x() - (*it).x()));
                            (*it).add_vsepy(-((*itt).y() - (*it).y()));
                        }

                    }
                }
                
                if ((*it).get_nearbysize() != 0)
                {
                    (*it).add_vcohex((x_cm / ((*it).get_nearbysize())) - (*it).x());
                    (*it).add_vcohey((y_cm / ((*it).get_nearbysize())) - (*it).y());
                    (*it).mult_valig(a_/(*it).get_nearbysize());
                }
                (*it).mult_vsep(s_);
                (*it).mult_vcohe(c_);
            }
        }
        else if (flock_.size() <= 1){throw std::runtime_error{"Must add at least two boid to the flock"};}
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

    double mp::Flock::get_mean_velocity(){return std::hypot(mean_vx_, mean_vy_);}
    double mp::Flock::get_stddeviation()
    {
        stddeviation_vx_ = 0.;
        stddeviation_vy_ = 0.;
        double sigma_squarex{0.};
        double sigma_squarey{0.};
        double mean_v{std::hypot(mean_vx_, mean_vy_)};
        for (auto it = flock_.begin(); it < flock_.end(); ++it)
        {
            sigma_squarex += std::pow((*it).vx() - mean_vx_, 2);
            sigma_squarey += std::pow((*it).vy() - mean_vy_, 2);
        }
        if ((flock_.size() > 1))
        {
            sigma_squarex /= (static_cast<double>(flock_.size()) * (static_cast<double>(flock_.size()) - 1.0));
            sigma_squarey /= (static_cast<double>(flock_.size()) * (static_cast<double>(flock_.size()) - 1.0));
            double sigma{std::pow(sigma_squarex*mean_vx_*mean_vx_ + sigma_squarey*mean_vy_*mean_vy_, 0.5)/mean_v};

            return sigma;
        }
        else{throw std::runtime_error{"Add at least two boids to the flock in ordwr to calculate the standard deviation of the mean velocity"};}
        
    }
    
}
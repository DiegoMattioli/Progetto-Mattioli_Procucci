#include <iostream>
#include <stdexcept>

#include "mattioli_procucci0.hpp"
#include "mattioli_procucci-grafica.hpp"

int main() {
  try {
    double d, ds, a, c, s;
    int n;
    std::cout << "\nInsert the number of Boids:\t";
    std::cin >> n;
    if (n < 0){
       throw std::runtime_error{
          "Inserted an invalid value for the number of boids"};
     }

    std::cout << "\nSelect the value of the parameter d (distance between the boids):\t";
    std::cin >> d;
    std::cout << "\nSelect the value of the parameter ds (distance of separation between the boids):\t";
    std::cin >> ds;
    std::cout << "\nSelect the value of the parameter 'a' (alignement of the flock, must be between 0 and 1):\t";
    std::cin >> a;
    std::cout << "\nSelect the value of the parameter 'c' (cohesione of the flock, must be between 0 and 1):\t";
    std::cin >> c;
    std::cout << "\nSelect the value of the parameter 's' (separation of the flock, must be between 0 and 1):\t";
    std::cin >> s;
    if (d < 0 || (ds < 0 || ds > d) || (a < 0 && a > 1) ||(c < 0 && c > 1) || (s < 0 && s > 1))
    {
      throw std::runtime_error
      {"\nInserted an invalid value for one of the parameters, all parameters must \n be between 0 and 1"};
    }
    else
    {
      mp::Parameters p{d, ds, s, a, c};
      mp::Flock f{p};
      f.add(n);
      df::draw_flock(f);
    }   
 } 
 catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } 
  catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}
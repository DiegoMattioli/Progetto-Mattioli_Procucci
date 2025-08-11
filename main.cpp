#include <iostream>
#include <stdexcept>

#include "mattioli_procucci0.hpp"

int main() {
  try {
    double d, ds, a, c, s;
    int n, rip;
    std::cout << "Insert the number of Boids";
    std::cin >> n;
    if (n < 0) {
      throw std::runtime_error{
          "Inserted an invalid value for the number of boids"};
    }

    std::cout << "Select the value of the parameter d";
    std::cin >> d;
    std::cout << "Select the value of the parameter ds";
    std::cin >> ds;
    std::cout << "Select the value of the parameter a";
    std::cin >> a;
    std::cout << "Select the value of the parameter c";
    std::cin >> c;
    std::cout << "Select the value of the parameter s";
    std::cin >> s;
    if ((d < 0 && d > 1) || (ds < 0 && ds > 1) || (a < 0 && a > 1) ||(c < 0 && c > 1) || (s < 0 && s > 1))
    {
      throw std::runtime_error
      {"\nInserted an invalid value for one of the parameters, all parameters must \n be between 0 and 1"};
    }
    else
    {
        mp::Parameters p{d, ds, s, a, c};
        mp::Flock f{p};
        f.add(n);
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
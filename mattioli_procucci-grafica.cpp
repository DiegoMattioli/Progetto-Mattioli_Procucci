#include <SFML/Graphics.hpp>
#include "mattioli_procucci-grafica.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>

namespace df
{
    void draw_flock(mp::Flock& f)
    {
        if (f.size() != 0)
        {
            sf::RenderWindow window(sf::VideoMode(610,610), "Simulation of the flock's behaviour");
            sf::Texture texture;
            sf::Sprite sprite;
            window.setFramerateLimit(5);

            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        window.close();
                        std::cout << "closing window" << "\n";
                    }
                }

                window.clear();
                sf::Image image;
                image.create(201, 201, sf::Color::Black);
                for (int i = 0; i < f.size(); ++i)
                {
                    unsigned int x = static_cast<unsigned int>(std::floor(f.get_positionx(i)));
                    unsigned int y = static_cast<unsigned int>(std::floor(f.get_positiony(i)));
                    if (x <= 200 && y < 200)
                    {
                        image.setPixel(x, y, sf::Color::White);
                    }
                }
                f.update_position(1);
                f.update_velocity();

                texture.loadFromImage(image);
                sprite.setTexture(texture);
                sprite.setScale(3.0f,3.0f);
                window.draw(sprite);
                window.display();
            }
        }
        
        else{throw std::runtime_error{"Flock is empity, add at least one Boid object to the flock"};}
    }

    void draw_flock(mp::Flock& f0, mp::Flock& f1)
    {
        if (f0.size() != 0 && f1.size() != 0)
        {
            sf::RenderWindow window(sf::VideoMode(600,600), "Simulation of the flock's behaviour");
            sf::Texture texture;
            sf::Sprite sprite;
            window.setFramerateLimit(5);

            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        window.close();
                        std::cout << "closing window" << "\n";
                    }
                }

                window.clear();
                sf::Image image;
                image.create(200, 200, sf::Color::Black);
                auto max_size{std::max(f0.size(), f1.size())};
                for (int i = 0; i < max_size; ++i)
                {
                    if (i < f0.size() && i < f1.size())
                    {
                        unsigned int x0 = static_cast<unsigned int>(std::floor(f0.get_positionx(i)));
                        unsigned int y0 = static_cast<unsigned int>(std::floor(f0.get_positiony(i)));
                        unsigned int x1 = static_cast<unsigned int>(std::floor(f1.get_positionx(i)));
                        unsigned int y1 = static_cast<unsigned int>(std::floor(f1.get_positiony(i)));
                        //std::cout << f1.is_predator(i) << "\t" << f0.is_predator(i)<< "\n";
                        if (x0 <= 200 && y0 < 200 && x1 < 200 && y1 < 200)
                        {
                            image.setPixel(x0, y0, sf::Color::Magenta);
                            image.setPixel(x1, y1, sf::Color::Cyan);
                        }
                    }
                    
                    else if (i >= f0.size() && i < f1.size())
                    {
                        unsigned int x1 = static_cast<unsigned int>(std::floor(f1.get_positionx(i)));
                        unsigned int y1 = static_cast<unsigned int>(std::floor(f1.get_positiony(i)));
                        if (x1 < 200 && y1 < 200)
                        {
                            image.setPixel(x1, y1, sf::Color::Cyan);
                        }
                    }

                    else
                    {
                        unsigned int x0 = static_cast<unsigned int>(std::floor(f0.get_positionx(i)));
                        unsigned int y0 = static_cast<unsigned int>(std::floor(f0.get_positiony(i)));
                        if (x0 < 200 && y0 < 200)
                        {
                            image.setPixel(x0, y0, sf::Color::Magenta);
                        }
                    }
                    
                }
                f0.update_position(1);
                f0.update_velocity();
                f1.update_position(1);
                f1.update_velocity();

                texture.loadFromImage(image);
                sprite.setTexture(texture);
                sprite.setScale(3.0f,3.0f);
                window.draw(sprite);
                window.display();
            }
        }

        else{throw std::runtime_error{"Flock is empity, add at least one Boid object to the flock"};}
    }
}
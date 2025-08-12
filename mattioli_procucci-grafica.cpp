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
            sf::RenderWindow window(sf::VideoMode(400,400), "Titolo finestra");
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
                image.create(100, 100, sf::Color::Black);
                for (int i = 0; i < f.size(); ++i)
                {
                    unsigned int x = static_cast<unsigned int>(std::floor(f.get_positionx(i)));
                    unsigned int y = static_cast<unsigned int>(std::floor(f.get_positiony(i)));
                    image.setPixel(x, y, sf::Color::White);
                }
                f.update_position(1);
                f.update_velocity();

                texture.loadFromImage(image);
                sprite.setTexture(texture);
                sprite.setScale(4.0f,4.0f);
                window.draw(sprite);
                window.display();
            }
        }
        
        else{throw std::runtime_error{"Flock is empity, add at least one Boid object to the flock"};}
    }
}
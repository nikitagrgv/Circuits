#pragma once

#include "SFML/Graphics/Font.hpp"
#include <iostream>

class Globals
{
public:
    Globals()
    {
        if (!font_.loadFromFile("consolas.ttf"))
        {
            std::cout << "Font not found" << std::endl;
        }
    }

    static Globals &get()
    {
        static Globals instance;
        return instance;
    }

    static const sf::Font &getFont() { return get().font_; }


private:
    sf::Font font_;
};

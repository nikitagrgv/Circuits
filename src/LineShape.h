#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>

class LineShape : public sf::RectangleShape
{
public:
    LineShape(sf::Vector2f p0, sf::Vector2f p1, float thickness = 1.f,
        const sf::Color &color = sf::Color::White);

    void setBegin(sf::Vector2f p);
    void setEnd(sf::Vector2f p);
    void setLine(sf::Vector2f p0, sf::Vector2f p1);
    void setThickness(float thickness);

private:
    void update_line();
    static float to_degrees(float radians);

private:
    sf::Vector2f p0_{};
    sf::Vector2f p1_{};
    float thickness_{1};
}
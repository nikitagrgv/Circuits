#include "LineShape.h"

#include "MathUtils.h"

LineShape::LineShape(sf::Vector2f p0, sf::Vector2f p1, float thickness, const sf::Color &color)
    : p0_(p0)
    , p1_(p1)
    , thickness_(thickness)
{
    setFillColor(color);
    update_line();
}

void LineShape::setBegin(sf::Vector2f p)
{
    p0_ = p;
    update_line();
}

void LineShape::setEnd(sf::Vector2f p)
{
    p1_ = p;
    update_line();
}

void LineShape::setLine(sf::Vector2f p0, sf::Vector2f p1)
{
    p0_ = p0;
    p1_ = p1;
    update_line();
}

void LineShape::setThickness(float thickness)
{
    thickness_ = thickness;
    update_line();
}

void LineShape::update_line()
{
    const sf::Vector2f delta = p1_ - p0_;
    const float length = Math::length(delta);
    const float angle = Math::toDegrees(std::atan2f(-delta.x, delta.y));

    setSize({thickness_, length});
    setPosition(p0_);
    setOrigin(thickness_ / 2, 0);
    setRotation(angle);
}

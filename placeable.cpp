#include "Placeable.h"

Placeable::Placeable(const QString& name,
                     const QColor& color,
                     float width,
                     float height,
                     float density,
                     float friction,
                     float restitution)
    : name(name), color(color), width(width), height(height),
    density(density), friction(friction), restitution(restitution), body(nullptr) {}

void Placeable::assignBody(b2Body* body)
{
    this->body = body;
}

b2Body* Placeable::getBody() const
{
    return body;
}

QString Placeable::getName() const
{
    return name;
}

QColor Placeable::getColor() const
{
    return color;
}

void Placeable::setName(const QString& name)
{
    this->name = name;
}

void Placeable::setColor(const QColor& color)
{
    this->color = color;
}

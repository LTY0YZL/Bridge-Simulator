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

// Method to create a Box2D body
b2Body* Placeable::createBody(b2World* world, float posX, float posY)
{
    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posX, posY);

    b2Body* newBody = world->CreateBody(&bodyDef);

    // Define the box shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2.0f, height / 2.0f);

    // Define the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    // Attach the fixture to the body
    newBody->CreateFixture(&fixtureDef);

    // Assign the created body to this Placeable
    assignBody(newBody);

    return newBody;
}


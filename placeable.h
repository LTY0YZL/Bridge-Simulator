#ifndef PLACEABLE_H
#define PLACEABLE_H

#include <QString>
#include <QColor>
#include <Box2D/Box2D.h>

class Placeable
{
public:
    Placeable(const QString& name,
              const QColor& color,
              float width = 1.0f,
              float height = 1.0f,
              float density = 0.5f,
              float friction = 0.5f,
              float restitution = 0.5f);

    // Method to create a Box2D body in the given world
    b2Body* createBody(b2World* world, float posX, float posY);

    // Accessors
    QString getName() const;
    QColor getColor() const;
    b2Body* getBody() const;

    // Mutators
    void setName(const QString& name);
    void setColor(const QColor& color);

    // Assign Box2D body to Placeable
    void assignBody(b2Body* body);

private:
    QString name;       // Name of the object
    QColor color;       // Color of the object
    float width;        // Width of the object
    float height;       // Height of the object
    float density;      // Density for Box2D fixture
    float friction;     // Friction for Box2D fixture
    float restitution;  // Restitution (bounciness) for Box2D fixture
    b2Body* body;       // Pointer to the associated Box2D body
};

#endif // PLACEABLE_H

#ifndef PLACEABLE_H
#define PLACEABLE_H

#include <QString>
#include <QColor>
#include <Box2D/Box2D.h>

class Placeable
{
public:
    // Constructor for creating a Placeable with default Box2D parameters
    Placeable(const QString& name,
              const QColor& color,
              float width = 1.0f,
              float height = 1.0f,
              float density = 0.5f,
              float friction = 0.5f,
              float restitution = 0.5f);

    // Assign a Box2D body to this Placeable
    void assignBody(b2Body* body);

    // Accessors
    b2Body* getBody() const;
    QString getName() const;
    QColor getColor() const;

    // Setters
    void setName(const QString& name);
    void setColor(const QColor& color);

    // Placeable properties
    QString name;    // Name of the object
    QColor color;    // Color of the object
    float width;     // Width of the object
    float height;    // Height of the object
    float density;   // Density for Box2D fixture
    float friction;  // Friction for Box2D fixture
    float restitution; // Restitution (bounciness) for Box2D fixture

private:
    b2Body* body;    // Box2D body associated with this Placeable
};

#endif // PLACEABLE_H

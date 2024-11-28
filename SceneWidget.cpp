#include "SceneWidget.h"
#include <QPainter>
#include <QDebug>

SceneWidget::SceneWidget(QWidget *parent)
    : QWidget(parent),
    world(b2Vec2(0.0f, -10.0f)),
    timer(this)
{
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f); // Set ground at y = 0 (bottom of the Box2D world)

    // Create the ground body
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f); // 50 units wide, 10 units tall

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Store the ground body for drawing
    this->groundBody = groundBody;

    // Define the dynamic body.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 20.0f); // Start the dynamic box 20 units above the ground

    body = world.CreateBody(&bodyDef);

    // Define the dynamic box shape.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f); // 1x1 box

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.1; // Add some bounciness

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    printf("Init world\n");

    connect(&timer, &QTimer::timeout, this, &SceneWidget::updateWorld);
    timer.start(10);
}

void SceneWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Paint the background
    paintBackground(painter);

    // Apply transformations to align Box2D with Qt's coordinate system
    painter.translate(width() / 2, height()); // Move origin to the center bottom
    painter.scale(1, -1);                     // Flip the y-axis for proper alignment

    // Draw all the Box2D shapes
    drawShape(painter, groundBody, Qt::green); // Draw the ground in green
    drawShape(painter, body, Qt::blue);        // Draw the dynamic body in blue
}

void SceneWidget::paintBackground(QPainter &painter)
{
    // Paint the background in light gray
    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawRect(0, 0, width(), height());
}

void SceneWidget::drawShape(QPainter &painter, const b2Body* body, const QColor &color)
{
    painter.setBrush(color);

    // Iterate through all fixtures attached to the body
    for (const b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        const b2Shape* shape = fixture->GetShape();

        if (shape->GetType() == b2Shape::e_polygon)
        {
            const b2PolygonShape* polygon = static_cast<const b2PolygonShape*>(shape);

            QPolygonF qPolygon;
            for (int i = 0; i < polygon->m_count; ++i)
            {
                b2Vec2 vertex = body->GetWorldPoint(polygon->m_vertices[i]);
                QPointF qPoint(vertex.x * 20, vertex.y * 20); // Scale Box2D units to pixels
                qPolygon << qPoint;
            }
            painter.drawPolygon(qPolygon);
        }
    }
}

void SceneWidget::updateWorld()
{
    // Step the Box2D world
    world.Step(1.0 / 60.0, 6, 2);
    update();
}

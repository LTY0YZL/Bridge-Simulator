#include "SceneWidget.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

SceneWidget::SceneWidget(QWidget *parent)
    : QWidget(parent),
    world(b2Vec2(0.0f, -10.0f)),
    selectedBody(nullptr),
    timer(this) // Initialize selectedBody to nullptr
{
    setMouseTracking(true);
    // Define the ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f); // Set ground at y = 0 (bottom of the Box2D world)

    // Create the ground body
    groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape
    b2PolygonShape groundBox;
    groundBox.SetAsBox(worldSize.width() / 2.0f, worldSize.height() / 10.0f); // Adjust size dynamically


    // Add the ground fixture to the ground body
    groundBody->CreateFixture(&groundBox, 0.0f);
    Placeable wood("Wood", Qt::gray, 2.0f, 4.0f);
    // Create the first dynamic body
    createDynamicBody(wood, 0.0f, 20.0f);
    createDynamicBody(wood, 1.0f, 20.0f);
    createDynamicBody(wood, 2.0f, 20.0f);
    createDynamicBody(wood, 3.0f, 20.0f);
    connect(&timer, &QTimer::timeout, this, &SceneWidget::updateWorld);
    timer.stop();
}
void SceneWidget::addDynamicBody()
{
    Placeable wood("Wood", Qt::darkYellow, 1.0f, 4.0f);
    createDynamicBody(wood,2.0f, 30.0f);
    qDebug() << "Dynamic body added via addDynamicBody()";
}
void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());
    emit mouseMovedInWorld(worldPos.x(), worldPos.y()); // Emit signal with world coordinates
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos()); // Convert screen to world coordinates

    // Create a new Placeable object
    Placeable newPlaceable("Box", Qt::blue, 2.0f, 2.0f);

    // Create a Box2D body and associate it with the Placeable
    createDynamicBody(newPlaceable, worldPos.x(), worldPos.y());
    updateWorld();

    qDebug() << "Box created at Box2D world position (" << worldPos.x() << "," << worldPos.y() << ")";
}

void SceneWidget::createDynamicBody(Placeable& placeable, float posX, float posY)
{
    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posX, posY);

    // Create the Box2D body in the world
    b2Body* body = world.CreateBody(&bodyDef);

    // Define the box shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox(placeable.width / 2.0f, placeable.height / 2.0f);

    // Define the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = placeable.density;
    fixtureDef.friction = placeable.friction;
    fixtureDef.restitution = placeable.restitution;

    // Attach the fixture to the body
    body->CreateFixture(&fixtureDef);

    // Assign the created body to the Placeable
    placeable.assignBody(body);

    // Add the Placeable to the list
    placeables.push_back(placeable);

    //qDebug() << "Dynamic body created: Name =" << placeable.getName()
    //         << ", Color =" << placeable.getColor().name()
    //         << ", Position = (" << posX << "," << posY << ")";
}


void SceneWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Paint the background
    paintBackground(painter);

    // Apply transformations to align Box2D with Qt's coordinate system
    painter.translate(width() / 2, height()); // Move origin to the center bottom
    painter.scale(1, -1);                     // Flip the y-axis for proper alignment

    // Draw the ground body
    drawShape(painter, groundBody, Qt::magenta);

    // Draw all placeables
    for (const auto& placeable : placeables)
    {
        if (placeable.getBody()) // Ensure the Placeable has a valid Box2D body
        {
            drawShape(painter, placeable.getBody(), placeable.getColor());
        }
    }
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
                QPointF qPoint(vertex.x * worldScale, vertex.y * worldScale); // Scale Box2D to screen
                qPolygon << qPoint;
            }
            painter.drawPolygon(qPolygon);
        }
    }
}
void SceneWidget::startSimulation()
{
    if (!timer.isActive()) {
        timer.start(10); // Restart the timer if it's not already running
        qDebug() << "Simulation started.";
    }
}
QPointF SceneWidget::screenToWorld(const QPointF& screenPos) const
{
    float box2dX = (screenPos.x() - width() / 2) / worldScale; // Convert X to Box2D world
    float box2dY = -(screenPos.y() - height()) / worldScale;   // Convert Y to Box2D world
    return QPointF(box2dX, box2dY);
}

void SceneWidget::updateWorld()
{
    // Only step the Box2D world if simulation is active
    if (timer.isActive()) {
        world.Step(1.0 / 60.0, 6, 2);
    }
    update();
}

#include "SceneWidget.h"
#include "qtimer.h"
#include <QPainter>
#include <QMouseEvent>

SceneWidget::SceneWidget(GameLevel* level, QWidget* parent)
    : QWidget(parent), simulationTimer(new QTimer(this)), gameLevel(level)
{
    setMouseTracking(true);
    connect(simulationTimer, &QTimer::timeout, [this]() {
        gameLevel->stepWorld(1.0f / 60.0f, 6, 2); // Step the simulation
        update(); // Repaint the widget
    });
}

void SceneWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Paint the background
    paintBackground(painter);

    // Transform for Box2D world alignment
    painter.translate(width() / 2, height());
    painter.scale(1, -1);

    // Draw ground body
    drawShape(painter, gameLevel->getGroundBody(), Qt::green);

    // Draw all dynamic bodies
    const auto& placeables = gameLevel->getPlaceables();
    for (const auto& placeable : placeables)
    {
        if (placeable.getBody())
        {
            drawShape(painter, placeable.getBody(), placeable.getColor());
        }
    }
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());

    // Create a new Placeable
    Placeable newPlaceable("Box", Qt::blue, 2.0f, 2.0f);

    // Delegate body creation to GameLevel
    gameLevel->createDynamicBody( newPlaceable, worldPos.x(), worldPos.y());
    update();
}
void SceneWidget::startSimulation()
{
    if (!simulationTimer->isActive()) {
        simulationTimer->start(16); // Start at ~60 FPS
    }
}
void SceneWidget::paintBackground(QPainter &painter)
{
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
                QPointF qPoint(vertex.x * 20, vertex.y * 20); // Scale
                qPolygon << qPoint;
            }
            painter.drawPolygon(qPolygon);
        }
    }
}

void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos()); // Convert screen coordinates to Box2D world coordinates
    emit mouseMovedInWorld(worldPos.x(), worldPos.y()); // Emit the signal with world coordinates
}

QPointF SceneWidget::screenToWorld(const QPointF& screenPos) const
{
    float box2dX = (screenPos.x() - width() / 2) / gameLevel->getWorldScale(); // Convert screen X to Box2D X
    float box2dY = -(screenPos.y() - height()) / gameLevel->getWorldScale();  // Convert screen Y to Box2D Y
    return QPointF(box2dX, box2dY);
}

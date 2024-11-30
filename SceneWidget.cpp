#include "SceneWidget.h"

SceneWidget::SceneWidget(GameLevel* level, QWidget* parent)
    : QWidget(parent),
    currentTool(0),
    worldScale(20.0f),
    simulationTimer(new QTimer(this)),
    gameLevel(level),
    isFirstPointSet(false)
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

    // Draw all ground bodies
    const auto& groundBodies = gameLevel->getGroundBodies();
    for (const b2Body* groundBody : groundBodies)
    {
        drawShape(painter, groundBody, Qt::green); // Paint ground bodies in green
    }

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

void SceneWidget::setWorldScale(float scale)
{
    if (scale > 0) {
        worldScale = scale;
    } else {
        qDebug() << "Invalid world scale. Scale must be positive.";
    }
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
                QPointF qPoint(vertex.x * worldScale, vertex.y * worldScale);
                qPolygon << qPoint;
            }
            painter.drawPolygon(qPolygon);
        }
    }
}

QPointF SceneWidget::screenToWorld(const QPointF& screenPos) const
{
    float box2dX = (screenPos.x() - width() / 2) / worldScale;
    float box2dY = -(screenPos.y() - height()) / worldScale;
    return QPointF(box2dX, box2dY);
}

void SceneWidget::setCurrentTool(int ID)
{

    currentTool=ID;
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF worldPos = screenToWorld(event->pos());
    auto& placeables = gameLevel->getPlaceables();
    if (event->button() == Qt::RightButton) // Right click Case
    {
        createGroundWithTwoPoints(worldPos);
        return;
    }
    if (currentTool == 0) // Left click Case
    {
        // Create a new Placeable at the clicked position
        Placeable newPlaceable("Box", 50, Qt::blue, 2.0f, 2.0f);
        gameLevel->createDynamicBody(newPlaceable, worldPos.x(), worldPos.y());
        update();
    }
    else if (currentTool == 1)
    {
        // Select tool: Change color of the object under the mouse
        Placeable* placeable = findPlaceableAt(worldPos, placeables);
        if (placeable)
        {
            placeable->setDisplayColor(QColor("forestgreen"));
            update();
        }
    }
    else if (currentTool == 2)
    {
        // Delete tool: Remove the object under the mouse
        auto it = findPlaceableIteratorAt(worldPos, placeables);
        if (it != placeables.end())
        {
            b2Body* body = it->getBody();
            if (body)
            {
                gameLevel->destroyBody(body);
            }
            placeables.erase(it);
            update();
        }
    }
}


void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF screenPos = event->pos();
    QPointF worldPos = screenToWorld(screenPos);
    emit mouseMovedInWorld(worldPos.x(), worldPos.y()); // Emit the signal with world coordinates
}
void SceneWidget::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        // Zoom in by increasing worldScale by 5%
        worldScale *= 1.05;
    } else {
        // Zoom out by decreasing worldScale by 5%
        worldScale /= 1.05;
    }

    // Clamp worldScale to avoid invalid values
    if (worldScale < 0.5f) {
        worldScale = 1.0f; // Minimum zoom level
    }
    else if (worldScale > 200.0f) {
        worldScale = 200.0f; // Minimum zoom level
    }

    qDebug() << "Zoomed to worldScale:" << worldScale;

    // Trigger a repaint with the updated scale
    update();
}
Placeable* SceneWidget::findPlaceableAt(const QPointF& worldPos, std::vector<Placeable>& placeables)
{
    for (auto& placeable : placeables)
    {
        b2Body* body = placeable.getBody();
        if (body)
        {
            for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            {
                if (fixture->TestPoint(b2Vec2(worldPos.x(), worldPos.y())))
                {
                    return &placeable;
                }
            }
        }
    }
    return nullptr; // No matching placeable found
}
std::vector<Placeable>::iterator SceneWidget::findPlaceableIteratorAt(
    const QPointF& worldPos, std::vector<Placeable>& placeables)
{
    for (auto it = placeables.begin(); it != placeables.end(); ++it)
    {
        b2Body* body = it->getBody();
        if (body)
        {
            for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            {
                if (fixture->TestPoint(b2Vec2(worldPos.x(), worldPos.y())))
                {
                    return it; // Get the vector contains the item want to deal with
                }
            }
        }
    }
    return placeables.end();
}
void SceneWidget::recordTwoWorldPoint(const QPointF& worldPos)
{
    if (!isFirstPointSet)
    {
        // Record the first point
        firstPoint = worldPos;
        isFirstPointSet = true;
    }
    else
    {
        // Record the second point
        secondPoint = worldPos;
        isFirstPointSet = false; // Reset for future use
        qDebug() << "Box2D World Points: First =" << firstPoint << ", Second =" << secondPoint;
    }
}
void SceneWidget::createGroundWithTwoPoints(const QPointF& worldPos)
{
    recordTwoWorldPoint(worldPos); // Record the point

    // If both points are recorded, create the ground body
    if (!isFirstPointSet) // Means second point was just set
    {
        // Calculate dimensions and center for the ground body
        float width = std::abs(secondPoint.x() - firstPoint.x());
        float height = std::abs(secondPoint.y() - firstPoint.y());
        float centerX = (firstPoint.x() + secondPoint.x()) / 2.0f;
        float centerY = (firstPoint.y() + secondPoint.y()) / 2.0f;

        qDebug() << "Creating ground body at center (" << centerX << "," << centerY
                 << ") with width =" << width << " and height =" << height;

        // Delegate ground body creation to GameLevel
        gameLevel->createGround(centerX, centerY, width, height);

        update(); // Update the scene to reflect changes
    }
}

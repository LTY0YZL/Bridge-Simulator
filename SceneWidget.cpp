#include "SceneWidget.h"
#include <algorithm>

SceneWidget::SceneWidget(GameLevel* level, QWidget* parent)
    : QWidget(parent),
    currentTool(0),
    worldScale(20.0f),
    simulationTimer(new QTimer(this)),
    gameLevel(level),
    showPreview(false),
    isFirstPointSet(false),
    panOffsetX(0.0f),   // Initialize the pan offsets
    panOffsetY(0.0f),
    isPanning(false)
{
    setMouseTracking(true);
    connect(simulationTimer, &QTimer::timeout, [this]() {
        gameLevel->stepWorld(1.0f / 60.0f, 6, 2);

        // Hail
        int hLevel = gameLevel->getHailLevel();
        if (hLevel > 0 && !hailSpawnPaused)
        {
            static int hailCounter = 0;
            hailCounter++;
            int spawnInterval = std::max(1, 60 / hLevel);
            if (hailCounter % spawnInterval == 0)
            {
                float hailSize = 0.2f + 0.2f * hLevel;
                float randomX = ((rand() % 20) - 10.0f);
                float spawnY = 40.0f;

                Placeable hailStone("Hail", 0, Qt::white, hailSize, hailSize, 0.1f, 0.3f, 0.1f);
                hailStone.setAsHail(true);
                hailStone.setCreationTime(QDateTime::currentDateTime());

                gameLevel->createDynamicBody(hailStone, randomX, spawnY);
            }
        }

        // Earthquake
        int eqLevel = gameLevel->getEarthquakeLevel();
        if (eqLevel > 0)
        {
            float shakeIntensity = (float)eqLevel * 0.5f;
            float shakeX = ((float)(rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
            float shakeY = ((float)(rand() % 100) / 100.0f - 0.5f) * shakeIntensity;

            panOffsetX = shakeX * worldScale;
            panOffsetY = shakeY * worldScale;

            auto& placeables = gameLevel->getPlaceables();

            float nearGroundThreshold = 2.0f;
            float velocityThreshold = 0.1f;

            for (auto &p : placeables)
            {
                b2Body* body = p.getBody();
                if (!body) continue;

                if (p.getName() == "Hail")
                {
                    b2Vec2 pos = body->GetPosition();
                    b2Vec2 vel = body->GetLinearVelocity();

                    if (pos.y <= nearGroundThreshold && fabs(vel.y) < velocityThreshold)
                    {
                        float impulse = ((float)(rand() % 100) / 100.0f - 0.5f) * (eqLevel * 0.5f);
                        body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
                    }
                }
                else
                {
                    float impulse = ((float)(rand() % 100) / 100.0f - 0.5f) * (eqLevel * 0.5f);
                    body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
                }
            }
        }
        else
        {
            panOffsetX = 0;
            panOffsetY = 0;
        }

        // Remove hail older than 10 seconds
        {
            auto& placeables = gameLevel->getPlaceables();
            QDateTime now = QDateTime::currentDateTime();

            placeables.erase(std::remove_if(placeables.begin(), placeables.end(),
                                            [this, &now](const Placeable& p) {
                                                if (p.isHailStone()) {
                                                    qint64 elapsedMs = p.getCreationTime().msecsTo(now);
                                                    if (elapsedMs > 15000) {
                                                        if (b2Body* body = p.getBody()) {
                                                            gameLevel->destroyBody(body);
                                                        }
                                                        return true;
                                                    }
                                                }
                                                return false;
                                            }), placeables.end());
        }

        update(); // Repaint the widget
    });
}

void SceneWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Paint the background
    paintBackground(painter);

    // Translate the painter to apply pan offsets
    painter.translate(width() / 2 + panOffsetX, height() + panOffsetY);

    // Apply scaling to make y-axis point upwards
    painter.scale(1, -1);

    // Draw all ground bodies
    const auto& groundBodies = gameLevel->getGroundBodies();
    for (const b2Body* groundBody : groundBodies)
    {
        drawShape(painter, groundBody, Qt::green);
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

    // Draw the preview if applicable
    if (showPreview && isFirstPointSet)
    {
        areaPreview(painter, firstPoint, currentMousePos);
        linePreview(painter, firstPoint, currentMousePos);
    }
}

void SceneWidget::setWorldScale(float scale) // World scale for zooming
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

void SceneWidget::stopSimulation()
{
    if (simulationTimer->isActive())
    {
        simulationTimer->stop();
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
                QPointF qPoint = box2DWorldToScreen(vertex);
                qPolygon << qPoint;
            }
            painter.drawPolygon(qPolygon);
        }
        else if (shape->GetType() == b2Shape::e_circle)
        {
            const b2CircleShape* circle = static_cast<const b2CircleShape*>(shape);
            b2Vec2 center = body->GetWorldPoint(circle->m_p);
            float radius = circle->m_radius * worldScale;

            QPointF screenCenter = box2DWorldToScreen(center);
            painter.drawEllipse(screenCenter, radius, radius);
        }
    }
}

QPointF SceneWidget::screenToWorld(const QPointF& screenPos) const
{
    // Adjust screen position by pan offset before applying transformation
    float box2dX = (screenPos.x() - (width() / 2 + panOffsetX)) / worldScale;
    float box2dY = (height() + panOffsetY - screenPos.y()) / worldScale;
    return QPointF(box2dX, box2dY);
}
QPointF SceneWidget::box2DWorldToScreen(const b2Vec2& worldPos) const
{
    float screenX = (worldPos.x * worldScale) ;
    float screenY = (worldPos.y * worldScale);
    return QPointF(screenX, screenY);
}


void SceneWidget::setCurrentTool(int ID)
{

    currentTool=ID;
}

void SceneWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        isPanning = true;               // Start the panning operation
        lastMousePos = event->pos();    // Store the initial position of the mouse
    }
    else
    {
        // Existing code for other tools
        QPointF worldPos = screenToWorld(event->pos());

        if (event->button() == Qt::RightButton)
        {
            if (currentTool == -1)
            {
                createGroundWithTwoPoints(worldPos);
                showPreview = isFirstPointSet; // Show preview only after the first point is set
                return;
            }
            else if (currentTool == -2) // Tool to delete ground objects
            {
                deleteGroundAt(worldPos);
                return;
            }
        }

        auto& placeables = gameLevel->getPlaceables();
        if (currentTool == 0) // Left click Case
        {
            Placeable newPlaceable("Box", 50, Qt::blue, 2.0f, 2.0f);
            gameLevel->createDynamicBody(newPlaceable, worldPos.x(), worldPos.y());
            update();
        }
        else if (currentTool == 1)
        {
            Placeable* placeable = findPlaceableAt(worldPos, placeables);
            if (placeable)
            {
                placeable->setDisplayColor(QColor("forestgreen"));
                update();
            }
        }
        else if (currentTool == 2)
        {
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
}

void SceneWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (isPanning)
    {
        // Calculate the difference between the current and last mouse position
        QPoint delta = event->pos() - lastMousePos;

        // Update the pan offsets
        panOffsetX += delta.x();
        panOffsetY += delta.y();

        // Update the last mouse position
        lastMousePos = event->pos();

        // Trigger repaint to reflect the panning
        update();
    }
    else
    {
        QPointF screenPos = event->pos();
        QPointF worldPos = screenToWorld(screenPos);

        if (isFirstPointSet) // Update preview if the first point is set
        {
            currentMousePos = worldPos;
            showPreview = true;
            update(); // Repaint to show the preview
        }

        emit mouseMovedInWorld(worldPos.x(), worldPos.y()); // Emit the signal with world coordinates
    }
}

void SceneWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        isPanning = false; // End the panning operation
    }
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
    recordTwoWorldPoint(worldPos);

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
        gameLevel->createGround(centerX, centerY, width, height);

        update();
    }
}
void SceneWidget::deleteGroundAt(const QPointF& worldPos)
{
    b2Body* groundBody = findGroundAt(worldPos);
    if (groundBody)
    {
        gameLevel->destroyGround(groundBody); // Call GameLevel to remove it
        update();
    }
}

b2Body* SceneWidget::findGroundAt(const QPointF& worldPos) const
{
    const auto& groundBodies = gameLevel->getGroundBodies();
    for (b2Body* body : groundBodies)
    {
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            if (fixture->TestPoint(b2Vec2(worldPos.x(), worldPos.y())))
            {
                return body;
            }
        }
    }
    return nullptr; // No ground object found
}
void SceneWidget::areaPreview(QPainter& painter, const QPointF& point1, const QPointF& point2)
{
    float width = std::abs(point2.x() - point1.x());
    float height = std::abs(point2.y() - point1.y());
    float centerX = (point1.x() + point2.x()) / 2.0f;
    float centerY = (point1.y() + point2.y()) / 2.0f;

    QRectF previewRect(
        (centerX - width / 2.0f) * worldScale,
        (centerY - height / 2.0f) * worldScale,
        width * worldScale,
        height * worldScale
        );

    painter.setBrush(QBrush(Qt::yellow, Qt::DiagCrossPattern));
    painter.setPen(Qt::DashLine);
    painter.drawRect(previewRect);
}
void SceneWidget::linePreview(QPainter& painter, const QPointF& start, const QPointF& end)
{
    QPen dashedPen(Qt::DashLine);
    dashedPen.setColor(Qt::red); // Set the color to red
    dashedPen.setWidth(2);
    painter.setPen(dashedPen);

    // Apply world-to-screen transformation
    QPointF screenStart(start.x() * worldScale, start.y() * worldScale);
    QPointF screenEnd(end.x() * worldScale, end.y() * worldScale);

    // Draw the line
    painter.drawLine(screenStart, screenEnd);
}




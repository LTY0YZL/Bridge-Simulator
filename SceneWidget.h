#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <vector>

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(QWidget *parent = nullptr);

    // Public method to create a dynamic body
    void createDynamicBody(float posX, float posY, float width, float height);
    QPointF screenToWorld(const QPointF& screenPos) const;

public slots:
    void addDynamicBody();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void mouseMovedInWorld(float x, float y);
private slots:
    void updateWorld();

private:
    void paintBackground(QPainter &painter);
    void drawShape(QPainter &painter, const b2Body* body, const QColor &color);

    b2World world;                      // Box2D world object
    b2Body* groundBody;                 // Ground body
    b2Body* selectedBody;               // Selected body
    std::vector<b2Body*> dynamicBodies; // List of all dynamic bodies
    QTimer timer;                       // Timer to step the Box2D world
    float worldScale = 20.0f;           // World scale factor (pixels per Box2D unit)
    QSizeF worldSize = QSizeF(100.0f, 50.0f); // World size (Box2D units)
};

#endif // SCENEWIDGET_H

#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <vector>
#include <Box2D/Box2D.h>

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(QWidget *parent = nullptr);

    // Public methods
    void applyForceToBox(const b2Vec2& force);
    void createDynamicBody(float posX, float posY, float width, float height);

protected:
    void paintEvent(QPaintEvent *event) override;

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
};

#endif // SCENEWIDGET_H

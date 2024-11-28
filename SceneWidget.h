#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <Box2D/Box2D.h>

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateWorld();

private:
    void paintBackground(QPainter &painter);              // Helper function to paint the background
    void drawShape(QPainter &painter, const b2Body* body, const QColor &color); // Helper to draw a shape

    b2World world;          // Box2D world object
    b2Body* body;           // Dynamic body in the Box2D world
    b2Body* groundBody;     // Ground body
    QTimer timer;           // Timer to step the Box2D world
};

#endif // SCENEWIDGET_H

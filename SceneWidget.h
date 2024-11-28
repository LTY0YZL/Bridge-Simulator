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

    // Public slot to handle adding a new dynamic body (used in MainWindow)
public slots:
    void addDynamicBody();

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

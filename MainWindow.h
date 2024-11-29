#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SceneWidget.h" // Include the SceneWidget class

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void updateMouseLocation(float x, float y); // Slot to update mouse location in QLabel
    void on_pushButton_8_clicked();

    void on_playButton_clicked();

private:
    Ui::MainWindow *ui;         // Pointer to the UI object
    SceneWidget *sceneWidget;   // Pointer to the SceneWidget instance
};

#endif // MAINWINDOW_H

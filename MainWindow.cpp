#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SceneWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    sceneWidget(new SceneWidget(this)) // Create the SceneWidget instance
{
    ui->setupUi(this);

    // Add SceneWidget to the UI layout
    ui->verticalLayout->addWidget(sceneWidget); // Assuming your .ui file has a verticalLayout for placement

    // Connect the button's clicked signal to SceneWidget's addDynamicBody slot
    connect(ui->pushButton, &QPushButton::clicked, sceneWidget, &SceneWidget::addDynamicBody);
    connect(sceneWidget, &SceneWidget::mouseMovedInWorld, this, &MainWindow::updateMouseLocation);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateMouseLocation(float x, float y)
{
    // Update the mouseLocation QLabel with the Box2D world coordinates
    ui->mouseLocation->setText(QString("X: %1, Y: %2").arg(x).arg(y));
}

void MainWindow::on_playButton_clicked()
{
    sceneWidget->startSimulation();

    ui->playButton->setEnabled(false);
}


void MainWindow::on_pushButton_8_clicked()
{

}


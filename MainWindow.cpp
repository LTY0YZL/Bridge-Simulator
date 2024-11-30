#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SceneWidget.h"
#include "GameLevel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    gameLevel(new GameLevel),
    sceneWidget(nullptr)
{
    ui->setupUi(this);

    // Initialize the game level
    gameLevel->initialize();

    // Create SceneWidget and pass GameLevel
    sceneWidget = new SceneWidget(gameLevel, this);
    ui->verticalLayout->addWidget(sceneWidget);

    // Connect the mouseMovedInWorld signal to update the label
    connect(sceneWidget, &SceneWidget::mouseMovedInWorld, this, &MainWindow::updateMouseLocation);
}

MainWindow::~MainWindow()
{
    delete gameLevel;
    delete ui;
}

void MainWindow::updateMouseLocation(float x, float y)
{
    // Update the QLabel with the mouse location
    ui->mouseLocation->setText(QString("X: %1, Y: %2").arg(x, 0, 'f', 2).arg(y, 0, 'f', 2));
}

void MainWindow::on_playButton_clicked()
{
    sceneWidget->startSimulation();
    ui->playButton->setEnabled(false);
}


void MainWindow::on_pushButton_clicked()
{
    float randomX = (rand() % 20) - 10.0f; // Random X position between -10 and 10
    float skyY = 30.0f; // Fixed Y position above the ground
    Placeable skyBox("SkyBox", 100, Qt::red,  0.5f, 0.5f); // Customize name, color, and size as needed

    // Add the Placeable to the GameLevel
    gameLevel->createDynamicBody(skyBox, randomX, skyY);

    // Update the SceneWidget to reflect the changes
    sceneWidget->update();
}


void MainWindow::on_selectTool_clicked()
{
    sceneWidget->setCurrentTool(1);
}


void MainWindow::on_boxToolButton_clicked()
{
    sceneWidget->setCurrentTool(0);
}


void MainWindow::on_deleteToolButton_clicked()
{
    sceneWidget->setCurrentTool(2);
}


void MainWindow::on_drawGroundButton_clicked()
{
    sceneWidget->setCurrentTool(-1);
}


void MainWindow::on_deleteGroundButton_clicked()
{
    sceneWidget->setCurrentTool(-2);
}


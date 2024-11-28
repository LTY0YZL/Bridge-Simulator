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
}

MainWindow::~MainWindow()
{
    delete ui;
}

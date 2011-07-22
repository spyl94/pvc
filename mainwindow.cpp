#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    i(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Projet4");
}

MainWindow::~MainWindow()
{
    delete ui;
}

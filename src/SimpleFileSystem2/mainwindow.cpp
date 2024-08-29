#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "inodesystem.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{


    INODESYSTEM* inode = new INODESYSTEM(8*1024);
   inode->createFile("test","t",512);
    inode->showFat();
}


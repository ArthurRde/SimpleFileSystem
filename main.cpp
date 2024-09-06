#include "mainwindow.h"

#include <QApplication>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QIcon>
#include <QDebug>


using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("File EXplorer");
    w.show();

    return a.exec();
}




#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inodesystem.h"
#include "fatSystem.h"
#include <string.h>
#include <sstream>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void createTableFileRows(QList<INode *> node);

    void showAllFolder(INODESYSTEM *sys, int rootId);

    void showFilesInFolder(INODESYSTEM *sys, string folderName);

    vector<int> splitStringIntoInts(string inputString);

    QTreeWidgetItem *setTreeWidgetChild(QTreeWidgetItem *rootItem, string name);

    QList<INode *> getFilesInFolder(INODESYSTEM *sys, string folderName);

    QList<INode *> getFoldersInFolder(INODESYSTEM *sys, string folderName);

    void setTreeWidgetChildRec(INODESYSTEM *sys, int rootId, QTreeWidgetItem *rootItem);

private
    slots:
            void on_treeWidget_DiskD_itemClicked(QTreeWidgetItem *item, int column);

            void on_tableWidget_cellDoubleClicked(int row);

            void on_pushButton_clicked();

        private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

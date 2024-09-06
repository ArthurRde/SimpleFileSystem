#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inodesystem.h"
#include "fatSystem.h"

#include <QTreeWidgetItem>

#include "dialogcreatefile.h"
#include "dialogshowfile.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    int slotSelected;
    INode* clipboardFileNode;
    bool clipboardNodeCopied = false;

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void createTableFileRows(QList<INode *> node);

    void showAllFolder(INODESYSTEM *sys, int rootId);

    void showFilesInFolder(INODESYSTEM *sys, string folderName);



    QTreeWidgetItem *setTreeWidgetChild(QTreeWidgetItem *rootItem, string name);



    void setTreeWidgetChildRec(INODESYSTEM *sys, int rootId, QTreeWidgetItem *rootItem);

    string currentFolder = "root";
    void showPath(INODESYSTEM *sys, int rootId);
    QString findFolderPath(INODESYSTEM *sys, int rootId, string foldername);
    int getSlotSelected() const;
    void setSlotSelected(int newSlotSelected);

    INode* getClipboardFileNode() const;
    void setClipboardFileNode( INode *newClipboardFileNode);

    bool getClipboardNodeCopied() const;
    void setClipboardNodeCopied(bool newClipboardNodeCopied);

    void showDataOfFile(INODESYSTEM *sys, string fileName);
public slots:
    void createDemoFiles();
private
    slots:
            void on_treeWidget_DiskD_itemClicked(QTreeWidgetItem *item, int column);

            void on_tableWidget_cellDoubleClicked(int row);

            void on_pushButton_clicked();

            void on_pushButton_5_clicked();

            void on_tableWidget_itemSelectionChanged();

            void on_pushButton_2_clicked();

            void on_pushButton_3_clicked();

            void on_pushButton_4_clicked();

        private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

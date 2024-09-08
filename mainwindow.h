#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "inodesystem.h"
#include "fatSystem.h"


#include <QTreeWidgetItem>

#include "dialogcreatefile.h"
#include "dialogshowfile.h"
#include "dialogrename.h"
#include "cdrom.h"
#include "dialogeject.h"
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
    File* clipboardFileFat;
    bool clipboardFatCopied = false;
    int showedSystem = 0; //1 = Inode, 2 = Fat

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

    void showDataOfFile(FATSYSTEM *sys, char *fileName);
    void showFilesInFolder(FATSYSTEM *sys, string folderName);
    void createTableFileRows(QList<File *> files);
    void showAllFolder(FATSYSTEM *sys, char* rootName);
    void setTreeWidgetChildRec(FATSYSTEM *sys, char* rootName, QTreeWidgetItem *rootItem);
    QString findFolderPath(FATSYSTEM *sys, char *rootName, char *foldername);
    void showPath(FATSYSTEM *sys, char *rootName);
    void showDataOfFile(INODESYSTEM *sys, string fileName);
    void createAllFilesOfFolder(FATSYSTEM *srcSys, INODESYSTEM *desSys, string srcFolderName, string desFolderName);
    void createAllFilesOfFolder(INODESYSTEM *srcSys, FATSYSTEM *desSys, string srcFolderName, string desFolderName);

    void buyNewCD();


public slots:
    void createDemoFiles();
    void ejectCD();
    void insertCD();
     void burnCD();
    void AddFilesToCD();
    void readCD();
    void defragSystem();
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

            void on_treeWidget_DiskC_itemClicked(QTreeWidgetItem *item, int column);

        private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

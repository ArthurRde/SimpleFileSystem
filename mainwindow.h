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

/**
 * @class MainWindow
 * @brief The main window of the application.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

    int slotSelected; /**< Selected slot */
    INode *clipboardFileNode; /**< Clipboard file node */
    bool clipboardNodeCopied = false; /**< Clipboard node copied flag */
    File *clipboardFileFat; /**< Clipboard file for FAT system */
    bool clipboardFatCopied = false; /**< Clipboard FAT copied flag */
    int showedSystem = 0; /**< Showed system: 1 = Inode, 2 = Fat */

public:
    /**
     * @brief Constructor for MainWindow.
     * @param parent Pointer to the parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow.
     */
    ~MainWindow();

    /**
     * @brief Creates table rows for files.
     * @param node List of INode pointers.
     */
    void createTableFileRows(QList<INode *> node);

    /**
     * @brief Shows all folders in the INODESYSTEM.
     * @param sys Pointer to the INODESYSTEM.
     * @param rootId Root ID of the folder.
     */
    void showAllFolder(INODESYSTEM *sys, int rootId);

    /**
     * @brief Shows files in a folder.
     * @param sys Pointer to the INODESYSTEM.
     * @param folderName Name of the folder.
     */
    void showFilesInFolder(INODESYSTEM *sys, string folderName);

    /**
     * @brief Sets a child item in the tree widget.
     * @param rootItem Pointer to the root item.
     * @param name Name of the child item.
     * @return Pointer to the created child item.
     */
    QTreeWidgetItem *setTreeWidgetChild(QTreeWidgetItem *rootItem, string name);

    /**
     * @brief Recursively sets child items in the tree widget.
     * @param sys Pointer to the INODESYSTEM.
     * @param rootId Root ID of the folder.
     * @param rootItem Pointer to the root item.
     */
    void setTreeWidgetChildRec(INODESYSTEM *sys, int rootId, QTreeWidgetItem *rootItem);

    string currentFolder = "root"; /**< Current folder name */

    /**
     * @brief Shows the path of a folder.
     * @param sys Pointer to the INODESYSTEM.
     * @param rootId Root ID of the folder.
     */
    void showPath(INODESYSTEM *sys, int rootId);

    /**
     * @brief Finds the path of a folder.
     * @param sys Pointer to the INODESYSTEM.
     * @param rootId Root ID of the folder.
     * @param foldername Name of the folder.
     * @return Path of the folder as a QString.
     */
    QString findFolderPath(INODESYSTEM *sys, int rootId, string foldername);

    /**
     * @brief Gets the selected slot.
     * @return Selected slot.
     */
    int getSlotSelected() const;

    /**
     * @brief Sets the selected slot.
     * @param newSlotSelected New selected slot.
     */
    void setSlotSelected(int newSlotSelected);

    /**
     * @brief Gets the clipboard file node.
     * @return Pointer to the clipboard file node.
     */
    INode *getClipboardFileNode() const;

    /**
     * @brief Sets the clipboard file node.
     * @param newClipboardFileNode Pointer to the new clipboard file node.
     */
    void setClipboardFileNode(INode *newClipboardFileNode);

    /**
     * @brief Gets the clipboard node copied flag.
     * @return Clipboard node copied flag.
     */
    bool getClipboardNodeCopied() const;

    /**
     * @brief Sets the clipboard node copied flag.
     * @param newClipboardNodeCopied New clipboard node copied flag.
     */
    void setClipboardNodeCopied(bool newClipboardNodeCopied);

    /**
     * @brief Shows the data of a file in the FAT system.
     * @param sys Pointer to the FATSYSTEM.
     * @param fileName Name of the file.
     */
    void showDataOfFile(FATSYSTEM *sys, char *fileName);

    /**
     * @brief Shows files in a folder in the FAT system.
     * @param sys Pointer to the FATSYSTEM.
     * @param folderName Name of the folder.
     */
    void showFilesInFolder(FATSYSTEM *sys, string folderName);

    /**
     * @brief Creates table rows for files in the FAT system.
     * @param files List of File pointers.
     */
    void createTableFileRows(QList<File *> files);

    /**
     * @brief Shows all folders in the FAT system.
     * @param sys Pointer to the FATSYSTEM.
     * @param rootName Name of the root folder.
     */
    void showAllFolder(FATSYSTEM *sys, char *rootName);

    /**
     * @brief Recursively sets child items in the tree widget for the FAT system.
     * @param sys Pointer to the FATSYSTEM.
     * @param rootName Name of the root folder.
     * @param rootItem Pointer to the root item.
     */
    void setTreeWidgetChildRec(FATSYSTEM *sys, char *rootName, QTreeWidgetItem *rootItem);

    /**
     * @brief Finds the path of a folder in the FAT system.
     * @param sys Pointer to the FATSYSTEM.
     * @param rootName Name of the root folder.
     * @param foldername Name of the folder.
     * @return Path of the folder as a QString.
     */
    QString findFolderPath(FATSYSTEM *sys, char *rootName, char *foldername);

    /**
     * @brief Shows the path of a folder in the FAT system.
     * @param sys Pointer to the FATSYSTEM.
     * @param rootName Name of the root folder.
     */
    void showPath(FATSYSTEM *sys, char *rootName);

    /**
     * @brief Shows the data of a file in the INODESYSTEM.
     * @param sys Pointer to the INODESYSTEM.
     * @param fileName Name of the file.
     */
    void showDataOfFile(INODESYSTEM *sys, string fileName);

    /**
     * @brief Creates all files of a folder from FAT system to INODESYSTEM.
     * @param srcSys Pointer to the source FATSYSTEM.
     * @param desSys Pointer to the destination INODESYSTEM.
     * @param srcFolderName Name of the source folder.
     * @param desFolderName Name of the destination folder.
     */
    void createAllFilesOfFolder(FATSYSTEM *srcSys, INODESYSTEM *desSys, string srcFolderName, string desFolderName);

    /**
     * @brief Creates all files of a folder from INODESYSTEM to FAT system.
     * @param srcSys Pointer to the source INODESYSTEM.
     * @param desSys Pointer to the destination FATSYSTEM.
     * @param srcFolderName Name of the source folder.
     * @param desFolderName Name of the destination folder.
     */
    void createAllFilesOfFolder(INODESYSTEM *srcSys, FATSYSTEM *desSys, string srcFolderName, string desFolderName);

    /**
     * @brief Buys a new CD.
     */
    void buyNewCD();

public slots:
    /**
     * @brief Creates demo files.
     */
    void createDemoFiles();

    /**
     * @brief Ejects the CD.
     */
    void ejectCD();

    /**
     * @brief Inserts a CD.
     */
    void insertCD();

    /**
     * @brief Burns a CD.
     */
    void burnCD();

    /**
     * @brief Adds files to the CD.
     */
    void AddFilesToCD();

    /**
     * @brief Reads the CD.
     */
    void readCD();

    /**
     * @brief Defragments the system.
     */
    void defragSystem();

private slots:
    /**
     * @brief Handles item click in DiskD tree widget.
     * @param item Pointer to the clicked item.
     * @param column Column index of the clicked item.
     */
    void on_treeWidget_DiskD_itemClicked(QTreeWidgetItem *item, int column);

    /**
     * @brief Handles cell double click in the table widget.
     * @param row Row index of the double-clicked cell.
     */
    void on_tableWidget_cellDoubleClicked(int row);

    /**
     * @brief Handles click on the first push button.
     */
    void on_pushButton_clicked();

    /**
     * @brief Handles click on the fifth push button.
     */
    void on_pushButton_5_clicked();

    /**
     * @brief Handles item selection change in the table widget.
     */
    void on_tableWidget_itemSelectionChanged();

    /**
     * @brief Handles click on the second push button.
     */
    void on_pushButton_2_clicked();

    /**
     * @brief Handles click on the third push button.
     */
    void on_pushButton_3_clicked();

    /**
     * @brief Handles click on the fourth push button.
     */
    void on_pushButton_4_clicked();

    /**
     * @brief Handles item click in DiskC tree widget.
     * @param item Pointer to the clicked item.
     * @param column Column index of the clicked item.
     */
    void on_treeWidget_DiskC_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui; /**< Pointer to the UI */
};

#endif // MAINWINDOW_H
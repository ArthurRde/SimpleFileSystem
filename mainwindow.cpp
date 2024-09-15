#include "mainwindow.h"
#include "cdsystem.h"
#include "dialogwritecd.h"
#include "ui_dialogcreatefile.h"
#include "ui_mainwindow.h"

#include <QRandomGenerator>
#include "dialoginsert.h"

using namespace std;

Disk *diskD = new Disk(64, 512);
Disk *diskC = new Disk(64, 512);
INODESYSTEM *Isys = new INODESYSTEM(diskD->getSize(), diskD);
FATSYSTEM *fSys = new FATSYSTEM(diskC->getSize(), diskC);
CDSYSTEM *cSys = new CDSYSTEM(Isys, fSys);
vector<string> producer = {"samsong", "mapple", "nokkia"};
int cdport = 0;

int MainWindow::getSlotSelected() const {
    return slotSelected;
}

void MainWindow::setSlotSelected(int newSlotSelected) {
    slotSelected = newSlotSelected;
}

INode *MainWindow::getClipboardFileNode() const {
    return clipboardFileNode;
}

void MainWindow::setClipboardFileNode(INode *newClipboardFileNode) {
    clipboardFileNode = newClipboardFileNode;
}

bool MainWindow::getClipboardNodeCopied() const {
    return clipboardNodeCopied;
}

void MainWindow::setClipboardNodeCopied(bool newClipboardNodeCopied) {
    clipboardNodeCopied = newClipboardNodeCopied;
}

void MainWindow::ejectCD() {
    DialogEject dlg(cSys);
    dlg.setWindowTitle("Eject CDs");

    for (int i = 0; i < cSys->getInsertedCds().size(); i++) {
        dlg.addCdToComboBox(cSys->getInsertedCds()[i]->portName);
    }
    dlg.exec();
}

void MainWindow::insertCD() {
    DialogInsert dlg(cSys);
    dlg.setWindowTitle("Insert CD");
    for (int i = 0; i < cSys->getEjectedCds().size(); i++) {
        dlg.addCdToComboBox(cSys->getEjectedCds()[i]->portName);
    }
    dlg.exec();
}


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // Set the number of columns
    ui->tableWidget->setColumnCount(4);

    // Set the header labels
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Type" << "Name" << "Size" << "Date");

    // Set the first column to have a fixed width
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(0, 60); // Set the fixed width for the first column

    // Set the remaining columns to stretch
    for (int c = 1; c < ui->tableWidget->columnCount(); ++c) {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }


    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton->setEnabled(false);
    connect(ui->actionCreate_demofiles, &QAction::triggered, this, &MainWindow::createDemoFiles);
    connect(ui->actionEject, &QAction::triggered, this, &MainWindow::ejectCD);
    connect(ui->actionBuy_a_new_CD, &QAction::triggered, this, &MainWindow::buyNewCD);
    connect(ui->actionInsert, &QAction::triggered, this, &MainWindow::insertCD);
    connect(ui->actionBurn_Cd, &QAction::triggered, this, &MainWindow::burnCD);
    connect(ui->actionAdd_Files, &QAction::triggered, this, &MainWindow::AddFilesToCD);
    connect(ui->actionRead, &QAction::triggered, this, &MainWindow::readCD);
    connect(ui->actionDefragmentieren, &QAction::triggered, this, &MainWindow::defragSystem);

    // Setup
    ui->treeWidget_DiskD->setColumnCount(1);
    ui->treeWidget_DiskD->setHeaderLabels(QStringList() << "Disk D");
    ui->treeWidget_DiskC->setColumnCount(1);
    ui->treeWidget_DiskC->setHeaderLabels(QStringList() << "Disk C");

    int rootId = stoi(diskD->getBlocks().at(0));
    showAllFolder(Isys, rootId);
    showAllFolder(fSys, "root");
    showFilesInFolder(Isys, "root");
    showFilesInFolder(fSys, "root");
    currentFolder = "root";
    showedSystem = 2;
}


void MainWindow::AddFilesToCD() {
    if (ui->tableWidget->item(slotSelected, 1) != NULL) {
        if (showedSystem == 1) {
            string file = ui->tableWidget->item(slotSelected, 1)->text().toStdString();
            cSys->nodes.push_back(Isys->getNodes()[Isys->findFile(file)]);

        } else {
            cSys->files.push_back(fSys->findFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString().c_str()));
        }
    }
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::burnCD() {
    cSys->burnCD();
    if (showedSystem == 1) {
        showFilesInFolder(Isys, currentFolder);
        showDataOfFile(Isys, currentFolder);
    } else {
        showFilesInFolder(fSys, currentFolder);
        showDataOfFile(fSys, const_cast<char *>( currentFolder.c_str()));

    }
}

void MainWindow::readCD() {

    cSys->readCD(cSys->insertedCds[0], showedSystem);
    if (showedSystem == 1) {
        showFilesInFolder(Isys, currentFolder);
        showDataOfFile(Isys, currentFolder);
    } else {
        showFilesInFolder(fSys, currentFolder);
        showDataOfFile(fSys, const_cast<char *>( currentFolder.c_str()));

    }
}

void MainWindow::buyNewCD() {
    int elem = producer.size();

    string port = "Port " + to_string(cdport) + " (" + producer[QRandomGenerator::global()->bounded(elem)] + ")";
    cSys->addEjectedCd(port);
    cdport++;
}

QString MainWindow::findFolderPath(INODESYSTEM *sys, int rootId, string foldername) {
    QString path;
    QList < INode * > children = sys->getFoldersInFolder(sys->getNodes()[rootId]->name);
    if (children.contains(sys->getNodes()[sys->findFile(foldername)])) {
        return QString::fromStdString(sys->getNodes()[rootId]->name + "/" + foldername);
    }
    for (int i = 0; i < children.size(); i++) {

        QList < INode * > newChild = sys->getFoldersInFolder(children[i]->name);

        if (newChild.size() > 0) {
            QString returnValue = findFolderPath(sys, sys->findFile(children[i]->name), foldername);
            if (returnValue != "notFound") {

                path = QString::fromStdString(sys->getNodes()[rootId]->name) + "/" + returnValue;

                return path;
            }
        } else {
            return "notFound";
        }
    }
    return "notFound";
}

void MainWindow::showPath(INODESYSTEM *sys, int rootId) {
    QString val = findFolderPath(sys, rootId, currentFolder);
    if (val == "notFound") {
        ui->label_3->setText("D: ~/");
    } else {
        ui->label_3->setText("D: " + val);
    }
}


QString MainWindow::findFolderPath(FATSYSTEM *sys, char *rootName, char *foldername) {
    QString path;
    QList < File * > children = sys->getFoldersInFolder(rootName);
    if (children.contains(sys->findFile(foldername))) {
        string str;
        str.assign(rootName);
        return QString::fromStdString(str + "/" + foldername);

    }
    for (int i = 0; i < children.size(); i++) {
        QList < File * > newChild = sys->getFoldersInFolder(children[i]->name);

        if (newChild.size() > 0) {
            QString returnValue = findFolderPath(sys, children[i]->name, foldername);
            if (returnValue != "notFound") {
                string str;
                str.assign(rootName);
                path = QString::fromStdString(str + "/") + returnValue;

                return path;
            }
        } else {
            return "notFound";
        }
    }
    return "notFound";
}

void MainWindow::showPath(FATSYSTEM *sys, char *rootName) {
    char c[currentFolder.length() + 1];
    char *c_folder = c;
    strcpy(c_folder, currentFolder.c_str());
    QString val = findFolderPath(sys, rootName, c_folder);
    if (val == "notFound") {
        ui->label_3->setText("C: ~/");
    } else {
        ui->label_3->setText("C: " + val);
    }
}


void MainWindow::createTableFileRows(QList<INode *> node) {
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(node.size());
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Type" << "Name" << "Size" << "Date");
    for (int i = 0; i < node.size(); i++) {
        if (node.at(i) != NULL) {
            QTableWidgetItem *typeItem = new QTableWidgetItem;
            if(node.at(i)->isFolder == true){
                typeItem->setText(QString::fromStdString("Folder"));
            }else{
                typeItem->setText(QString::fromStdString("File"));
            }

            typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 0, typeItem);

            QTableWidgetItem *nameItem = new QTableWidgetItem;
            nameItem->setText(QString::fromStdString(node.at(i)->name));
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 1, nameItem);

            QTableWidgetItem *sizeItem = new QTableWidgetItem;
            sizeItem->setText(QString::number(node.at(i)->size / 1000) + " KB");
            sizeItem->setFlags(sizeItem->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 2, sizeItem);

            QTableWidgetItem *dateItem = new QTableWidgetItem;
            dateItem->setText(node.at(i)->date.toString("dd.MM.yyyy HH:mm:ss"));
            dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 3, dateItem);
        }
    }
}

void MainWindow::createTableFileRows(QList<File *> files) {
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(files.size());
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Type" << "Name" << "Size" << "Date");
    for (int i = 0; i < files.size(); i++) {

        QTableWidgetItem *typeItem = new QTableWidgetItem;
        if(files.at(i)->isFolder == true){
            typeItem->setText(QString::fromStdString("Folder"));
        }else{
            typeItem->setText(QString::fromStdString("File"));
        }
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 0, typeItem);

        QTableWidgetItem *nameItem = new QTableWidgetItem;
        nameItem->setText(QString::fromStdString(files.at(i)->name));
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 1, nameItem);

        QTableWidgetItem *sizeItem = new QTableWidgetItem;
        sizeItem->setText(QString::number(files.at(i)->size / 1000) + " KB");
        sizeItem->setFlags(sizeItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 2, sizeItem);

        QTableWidgetItem *dateItem = new QTableWidgetItem;
        dateItem->setText(files.at(i)->date.toString("dd.MM.yyyy HH:mm:ss"));
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 3, dateItem);

    }
}

//deprecated
QList<INode *> showAllFilesINode(INODESYSTEM *sys) {
    QList < INode * > node;
    for (int i = 0; i < sys->getNodes().size() + 1; i++) {
        if (sys->getNodes()[i] != NULL) {
            node.append(sys->getNodes()[i]);
        }
    }
    return node;

}

QTreeWidgetItem *MainWindow::setTreeWidgetChild(QTreeWidgetItem *rootItem, string name) {
    QTreeWidgetItem *childItem = new QTreeWidgetItem(rootItem);
    childItem->setText(0, QString::fromStdString(name));
    return childItem;
}

void MainWindow::setTreeWidgetChildRec(INODESYSTEM *sys, int rootId, QTreeWidgetItem *rootItem) {
    QList < INode * > children = sys->getFoldersInFolder(sys->getNodes()[rootId]->name);
    rootItem->setExpanded(true);
    for (int i = 0; i < children.size(); i++) {
        QTreeWidgetItem *item = setTreeWidgetChild(rootItem, children[i]->name);
        QList < INode * > newChild = sys->getFoldersInFolder(children[i]->name);
        if (newChild.size() > 0) {
            setTreeWidgetChildRec(sys, sys->findFile(children[i]->name), item);
        }
    }
}


void MainWindow::showAllFolder(INODESYSTEM *sys, int rootId) {
    ui->treeWidget_DiskD->clear();
    //root folder anzeigen
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget_DiskD);
    rootItem->setText(0, QString::fromStdString(sys->getNodes()[rootId]->name));
    setTreeWidgetChildRec(sys, rootId, rootItem);
    ui->progressBar->setValue(sys->getFragmentation());

    ui->label_4->setText(QString().append("Frag. ").append(QString::number(sys->getFragmentation(), 'f', 2).append("%")));
}


void MainWindow::setTreeWidgetChildRec(FATSYSTEM *sys, char *rootName, QTreeWidgetItem *rootItem) {
    QList < File * > children = sys->getFoldersInFolder(sys->findFile(rootName)->name);
    for (int i = 0; i < children.size(); i++) {
        QTreeWidgetItem *item = setTreeWidgetChild(rootItem, children[i]->name);
        rootItem->setExpanded(true);
        QList < File * > newChild = sys->getFoldersInFolder(children[i]->name);
        if (newChild.size() > 0) {
            setTreeWidgetChildRec(sys, children[i]->name, item);
        }
    }
}


void MainWindow::showAllFolder(FATSYSTEM *sys, char *rootName) {
    ui->treeWidget_DiskC->clear();
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget_DiskC);
    rootItem->setText(0, QString::fromStdString(sys->findFile("root")->name));
    setTreeWidgetChildRec(sys, rootName, rootItem);
    ui->progressBar->setValue(sys->getFragmentation());
    ui->label_4->setText(QString().append("Frag. ").append(QString::number(sys->getFragmentation(), 'f', 2).append("%")));
}


void MainWindow::showFilesInFolder(INODESYSTEM *sys, string folderName) {
    int fileId = sys->findFile(folderName);

    if (sys->getNodes()[fileId]->isFolder) {
        currentFolder = folderName;
        createTableFileRows(sys->getFilesInFolder(folderName));
        showPath(sys, stoi(diskD->getBlocks().at(0)));
        showedSystem = 1;
    }
}

void MainWindow::showFilesInFolder(FATSYSTEM *sys, string folderName) {
    if (sys->findFile(folderName.c_str())->isFolder) {
        currentFolder = folderName;
        QList <File> files;
        char c[folderName.length() + 1];
        char *c_folder = c;
        strcpy(c_folder, folderName.c_str());

        showPath(sys, "root");
        createTableFileRows(sys->getFilesInFolder(c_folder));
        showedSystem = 2;
    }
}


void MainWindow::on_treeWidget_DiskD_itemClicked(QTreeWidgetItem *item, int column) {
    showFilesInFolder(Isys, item->text(column).toStdString());
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row) {
    string fileName = ui->tableWidget->item(row, 1)->text().toStdString();
    if (showedSystem == 1) {
        showFilesInFolder(Isys, fileName);
        showDataOfFile(Isys, fileName);
    } else {
        showFilesInFolder(fSys, fileName);
        showDataOfFile(fSys, const_cast<char *>(fileName.c_str()));

    }
}


void MainWindow::showDataOfFile(INODESYSTEM *sys, string fileName) {
    int fileId = sys->findFile(fileName);
    if (!sys->getNodes()[fileId]->isFolder) {
        DialogShowFile dlg(sys->getNodes()[fileId]->name, sys->getNodes()[fileId]->author,
                           sys->getNodes()[fileId]->date, sys->getNodes()[fileId]->size, sys->getDataOfFile(fileName));
        dlg.setWindowTitle("File information");
        if (dlg.exec() == QDialog::Accepted) {
            sys->editData(fileId, dlg.getDataText());
        }
    }
    int rootId = stoi(diskD->getBlocks().at(0));
    showAllFolder(sys, rootId);
    showFilesInFolder(sys, currentFolder);
}

void MainWindow::showDataOfFile(FATSYSTEM *sys, char *fileName) {
    if (!sys->findFile(fileName)->isFolder) {
        DialogShowFile dlg(sys->findFile(fileName)->name, sys->findFile(fileName)->author,
                           sys->findFile(fileName)->date, sys->findFile(fileName)->size, sys->getDataOfFile(fileName));
        dlg.setWindowTitle("File information");
        if (dlg.exec() == QDialog::Accepted) {
            sys->editData(fileName, dlg.getDataText());
        }
    }
    showAllFolder(sys, "root");
    showFilesInFolder(sys, currentFolder);
}

char *stringToCharArray(const string &str) {
    char *char_array = new char[str.length() + 1];
    strcpy(char_array, str.c_str());
    return char_array;
}


void MainWindow::on_pushButton_clicked() {
    DialogRename dlg(ui->tableWidget->item(slotSelected, 1)->text().toStdString());
    dlg.setWindowTitle("Rename a File");
    dlg.show();
    if (dlg.exec() == QDialog::Accepted) {
        if (showedSystem == 1) {
            Isys->renameFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString(), dlg.getName());
            int rootId = stoi(diskD->getBlocks().at(0));
            showAllFolder(Isys, rootId);
            showFilesInFolder(Isys, currentFolder);
        } else {
            fSys->renameFile(stringToCharArray(currentFolder),
                             stringToCharArray(ui->tableWidget->item(slotSelected, 1)->text().toStdString()),
                             stringToCharArray(dlg.getName()));
            showAllFolder(fSys, "root");
            showFilesInFolder(fSys, currentFolder);
        }
    }
}


void MainWindow::on_pushButton_5_clicked() {
    DialogCreateFile dlg;
    dlg.setWindowTitle("Create a File");
    dlg.show();
    if (dlg.exec() == QDialog::Accepted) {
        if (showedSystem == 1) {
            if (dlg.getUi()->comboBox->currentText() == "Folder") {
                Isys->createFile(dlg.getUi()->lineEdit->text().toStdString(),
                                 dlg.getUi()->lineEdit_2->text().toStdString(), " ", currentFolder, true);
            } else {
                if (dlg.getUi()->comboBox->currentText() != "custom") {
                    string name = dlg.getUi()->lineEdit->text().toStdString() + "." +
                                  dlg.getUi()->comboBox->currentText().toStdString();
                    Isys->createFile(name, dlg.getUi()->lineEdit_2->text().toStdString(),
                                     dlg.getUi()->lineEdit_3->text().toStdString(), currentFolder);
                } else {
                    Isys->createFile(dlg.getUi()->lineEdit->text().toStdString(),
                                     dlg.getUi()->lineEdit_2->text().toStdString(),
                                     dlg.getUi()->lineEdit_3->text().toStdString(), currentFolder);
                }
            }
            int rootId = stoi(diskD->getBlocks().at(0));
            showAllFolder(Isys, rootId);
            showFilesInFolder(Isys, currentFolder);
        } else {
            if (dlg.getUi()->comboBox->currentText() == "Folder") {
                char *c_name = stringToCharArray(dlg.getUi()->lineEdit->text().toStdString());
                char *c_author = stringToCharArray(dlg.getUi()->lineEdit_2->text().toStdString());
                char *c_folder = stringToCharArray(currentFolder);
                fSys->createFile(c_name, c_author, " ", c_folder, true);
            } else {
                if (dlg.getUi()->comboBox->currentText() != "custom") {
                    string c_name = fSys->createUniqueName(
                            const_cast<char *>((dlg.getUi()->lineEdit->text().toStdString() + "." +
                                                dlg.getUi()->comboBox->currentText().toStdString()).c_str()), "-D");
                    char *c_author = stringToCharArray(dlg.getUi()->lineEdit_2->text().toStdString());
                    char *c_folder = stringToCharArray(currentFolder);
                    fSys->createFile(const_cast<char *>(c_name.c_str()), c_author,
                                     dlg.getUi()->lineEdit_3->text().toStdString(), c_folder);
                } else {
                    char *c_name = stringToCharArray(dlg.getUi()->lineEdit->text().toStdString());
                    char *c_author = stringToCharArray(dlg.getUi()->lineEdit_2->text().toStdString());
                    char *c_folder = stringToCharArray(currentFolder);
                    fSys->createFile(c_name, c_author, dlg.getUi()->lineEdit_3->text().toStdString(), c_folder);
                }
            }
            showAllFolder(fSys, "root");
            showFilesInFolder(fSys, currentFolder);
        }
    }
}


void MainWindow::on_tableWidget_itemSelectionChanged() {
    QItemSelectionModel *selectionModel = ui->tableWidget->selectionModel();
    if (selectionModel->hasSelection()) {
        QModelIndexList selectedRows = selectionModel->selectedRows();
        if (!selectedRows.isEmpty()) {
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton->setEnabled(true);
            slotSelected = selectedRows[0].row();
        } else {
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton->setEnabled(false);
        }
    } else {
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }
}

void MainWindow::on_pushButton_2_clicked() {
    if (showedSystem == 1) {
        Isys->deleteFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString());
        showFilesInFolder(Isys, currentFolder);
        showAllFolder(Isys, stoi(diskD->getBlocks().at(0)));
    } else {
        fSys->deleteFile(stringToCharArray(ui->tableWidget->item(slotSelected, 1)->text().toStdString()));
        showFilesInFolder(fSys, stringToCharArray(currentFolder));
        showAllFolder(fSys, "root");
    }
}


void MainWindow::on_pushButton_3_clicked() {
    ui->pushButton_4->setEnabled(true);
    if (showedSystem == 1) {
        clipboardNodeCopied = true;
        clipboardFatCopied = false;
        clipboardFileFat = NULL;
        clipboardFileNode = Isys->getNodes()[Isys->findFile(
                ui->tableWidget->item(slotSelected, 1)->text().toStdString())];
    } else {
        clipboardNodeCopied = false;
        clipboardFatCopied = true;
        clipboardFileNode = NULL;
        clipboardFileFat = fSys->findFile(
                stringToCharArray(ui->tableWidget->item(slotSelected, 1)->text().toStdString()));
    }
}


void MainWindow::on_pushButton_4_clicked() {
    if (clipboardNodeCopied) {
        if (showedSystem == 1) {
            Isys->copyFile(clipboardFileNode->name, currentFolder);
            showFilesInFolder(Isys, currentFolder);
            showAllFolder(Isys, stoi(diskD->getBlocks().at(0)));
        } else {
            if (!clipboardFileNode->isFolder) {
                fSys->createFile(const_cast<char *>(clipboardFileNode->name.c_str()),
                                 const_cast<char *>(clipboardFileNode->author.c_str()),
                                 Isys->getDataOfFile(clipboardFileNode->name),
                                 const_cast<char *>(currentFolder.c_str()));
                showFilesInFolder(fSys, currentFolder);
                showAllFolder(fSys, "root");
            } else {
                char *name = const_cast<char *>(fSys->createUniqueName(
                        const_cast<char *>(clipboardFileNode->name.c_str())).c_str());
                string name2 = fSys->createFile(name, const_cast<char *>(clipboardFileNode->author.c_str()), " ",
                                                const_cast<char *>(currentFolder.c_str()), true)->name;
                createAllFilesOfFolder(Isys, fSys, clipboardFileNode->name, name);
                showFilesInFolder(fSys, currentFolder);
                showAllFolder(fSys, "root");
            }
        }
    } else {
        if (showedSystem == 1) {
            if (!clipboardFileFat->isFolder) {
                Isys->createFile(clipboardFileFat->name, clipboardFileFat->author,
                                 fSys->getDataOfFile(clipboardFileFat->name), currentFolder);
                showFilesInFolder(Isys, currentFolder);
                showAllFolder(Isys, stoi(diskD->getBlocks().at(0)));
            } else {
                string name = Isys->createFile(clipboardFileFat->name, clipboardFileFat->author, " ", currentFolder,
                                               true);
                createAllFilesOfFolder(fSys, Isys, clipboardFileFat->name, name);
                showFilesInFolder(Isys, currentFolder);
                showAllFolder(Isys, stoi(diskD->getBlocks().at(0)));
            }
        } else {
            fSys->copyFile(clipboardFileFat->name, stringToCharArray(currentFolder));
            showFilesInFolder(fSys, currentFolder);
            showAllFolder(fSys, "root");
        }
    }
}

void
MainWindow::createAllFilesOfFolder(FATSYSTEM *srcSys, INODESYSTEM *desSys, string srcFolderName, string desFolderName) {
    QList < File * > files = srcSys->getFilesInFolder(const_cast<char *>(srcFolderName.c_str()));
    for (int i = 0; i < files.size(); i++) {
        if (!files[i]->isFolder) {
            desSys->createFile(files[i]->name, files[i]->author, srcSys->getDataOfFile(files[i]->name), desFolderName);
        } else {
            desSys->createFile(files[i]->name, files[i]->author, " ", desFolderName, true);
            createAllFilesOfFolder(srcSys, desSys, files[i]->name, files[i]->name);
        }
    }
}

void
MainWindow::createAllFilesOfFolder(INODESYSTEM *srcSys, FATSYSTEM *desSys, string srcFolderName, string desFolderName) {
    QList < INode * > files = srcSys->getFilesInFolder(const_cast<char *>(srcFolderName.c_str()));
    for (int i = 0; i < files.size(); i++) {
        if (!files[i]->isFolder) {
            desSys->createFile(const_cast<char *>(files[i]->name.c_str()), const_cast<char *>(files[i]->author.c_str()),
                               srcSys->getDataOfFile(files[i]->name), const_cast<char *>(desFolderName.c_str()));
        } else {
            desSys->createFile(const_cast<char *>(files[i]->name.c_str()), const_cast<char *>(files[i]->author.c_str()),
                               " ", const_cast<char *>(desFolderName.c_str()), true);
            createAllFilesOfFolder(srcSys, desSys, files[i]->name, files[i]->name);
        }
    }
}

void MainWindow::createDemoFiles() {
    int rootId = stoi(diskD->getBlocks().at(0));
    Isys->createFile("documents", "user", " ", "root", true);
    Isys->createFile("downloads", "system", " ", "root", true);
    Isys->createFile("keyAccess.closed", "KeyCode.exe", "bearer237746643698892388734794378478979847942794279842978974821", "root");
    Isys->createFile("poem.txt", "author", "Betriebssysteme, still und leise,Lenken uns auf ihre Weise.Windows, Linux, macOS,Jedes hat sein eigenes SOS.Sie starten Programme, verwalten Dateien,Lassen uns in digitale Welten eintauchen und verweilen.Mit Klicks und Tasten, so flink und schnell,Machen sie unser Leben digital und hell.Doch manchmal, oh, da gibt’s Probleme,Ein Absturz, ein Fehler, das sind die Themen.Doch wir wissen, sie sind stets bereit,Für uns zu arbeiten, Tag und Nacht, jederzeit.", "documents");
    Isys->createFile("examSolutions.pdf", "anonymus", "Frage: Erklären Sie das OSI-Modell und nennen Sie die sieben Schichten.solution: Das OSI-Modell (Open Systems Interconnection Model) ist ein Referenzmodell für die Kommunikation in Netzwerken. Es besteht aus sieben Schichten:Physikalische Schicht (Physical Layer)Sicherungsschicht (Data Link Layer)Netzwerkschicht (Network Layer)Transportschicht (Transport Layer)Sitzungsschicht (Session Layer)Darstellungsschicht (Presentation Layer)Anwendungsschicht (Application Layer)", "downloads");
    Isys->createFile("photos", "system", " ", "documents", true);
    Isys->createFile("summer.png", "apple", "A beautiful summer, A beautiful summer", "photos");

    Isys->defragDisk();
    showAllFolder(Isys, rootId);
    showFilesInFolder(Isys, "root");
    fSys->createFile("documents", "user", " ", "root", true);
    fSys->createFile("downloads", "system", " ", "root", true);
    fSys->createFile("Homework", "user", " ","documents", true);

    fSys->createFile("Sheet-1.txt","author", "1+1 = 2, 2+2 = 3, 4+4 = 8, 6+6 = 11, 9+9 = 98","Homework");
    fSys->createFile("Sheet-2.txt", "author", "f(x)=x+3 NS: x = 0", "Homework");

    fSys->createFile("beach.png", "excalidraw", "A nice warm day at the beach with some dolphins jumping out of the water", "downloads");

    fSys->showFat();
    fSys->defragDisk();
    showFilesInFolder(fSys, "root");
    showAllFolder(fSys, "root");
}

void MainWindow::on_treeWidget_DiskC_itemClicked(QTreeWidgetItem *item, int column) {
    showFilesInFolder(fSys, item->text(column).toStdString());
}

void MainWindow::defragSystem() {
    if (showedSystem == 1) {
        Isys->defragDisk();
        ui->progressBar->setValue(Isys->getFragmentation());
        ui->label_4->setText(QString().append("Frag. ").append(to_string(Isys->getFragmentation()).append("%")));
    } else {
        fSys->defragDisk();
        ui->progressBar->setValue(fSys->getFragmentation());
        ui->label_4->setText(QString().append("Frag. ").append(to_string(Isys->getFragmentation()).append("%")));
    }
}

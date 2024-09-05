#include "mainwindow.h"
#include "ui_mainwindow.h"


Disk *diskD = new Disk(16);
Disk *diskC = new Disk(16);
INODESYSTEM *sys = new INODESYSTEM(diskD->getSize(), diskD);
//FATSYSTEM *fSys = new FATSYSTEM();


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // Set the number of columns
    ui->tableWidget->setColumnCount(4);

    // Set the header labels
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Icon" << "Name" << "Size" << "Date");

    // Set the first column to have a fixed width
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(0, 50); // Set the fixed width for the first column

    // Set the remaining columns to stretch
    for (int c = 1; c < ui->tableWidget->columnCount(); ++c) {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Setup for Demo
    int rootId = stoi(diskD->getBlocks().at(0));
    sys->createFile("documents", "author", " ", "root", true);
    sys->createFile("home.txt", "author", "data", "root");
    sys->createFile("bath.txt", "author", "data", "documents");
    sys->createFile("downloads", "author", " ", "root", true);
    sys->createFile("file.pdf", "author", "data", "downloads");
    sys->createFile("photos", "author", " ", "documents", true);
    sys->createFile("file.png", "author", "data", "photos");

    //fSys->createFat(diskC->getBlocks().size(),diskC->getBlockSize());

    ui->treeWidget_DiskD->setColumnCount(1);
    ui->treeWidget_DiskD->setHeaderLabels(QStringList() << "Folders");
    showAllFolder(sys, rootId);
    showFilesInFolder(sys, "root");


    /** ;
     ui->treeWidget_DiskD->setColumnCount(1);
     ui->treeWidget_DiskD->setHeaderLabels(QStringList() << "Name");

     QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget_DiskD);
     rootItem->setText(0, "Projekt A");


     QTreeWidgetItem *childItem = new QTreeWidgetItem(rootItem);
     childItem->setText(0, "Datei 1");



     ui->tableWidget->setColumnCount(3);
     ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Icon" << "Name" << "Zahl");

     ui->tableWidget->setRowCount(1);

     QTableWidgetItem *iconItem = new QTableWidgetItem();
     iconItem->setIcon(QIcon("path/to/icon.png"));
     ui->tableWidget->setItem(0, 0, iconItem);

     QTableWidgetItem *nameItem = new QTableWidgetItem("Beispielname");
     ui->tableWidget->setItem(0, 1, nameItem);

     QTableWidgetItem *numberItem = new QTableWidgetItem("123");
     ui->tableWidget->setItem(0, 2, numberItem);

     ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
     ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
 */


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createTableFileRows(QList<INode *> node) {
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(node.size());
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Icon" << "Name" << "Size" << "Date");
    for (int i = 0; i < node.size(); i++) {
        if (node.at(i) != NULL) {
            QTableWidgetItem *iconItem = new QTableWidgetItem;
            iconItem->setIcon(QIcon("path/to/icon.png"));
            iconItem->setFlags(iconItem->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 0, iconItem);

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


//deprecated
QList<INode *> showAllFilesINode(INODESYSTEM *sys) {
    QList < INode * > node;
    for (int i = 0; i < sys->getNodes().size() + 1; i++) {
        if (sys->getNodes()[i] != NULL) {
            qDebug() << sys->getNodes()[i]->name;
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
    QList < INode * > children = getFoldersInFolder(sys, sys->getNodes()[rootId]->name);

    for (int i = 0; i < children.size(); i++) {
        QTreeWidgetItem *item = setTreeWidgetChild(rootItem, children[i]->name);
        QList < INode * > newChild = getFoldersInFolder(sys, children[i]->name);
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


    //liste mit folders

    //root
    //  documents
    //      photos

    /**QList<INode*> node;
    for(int i = 0; i< sys->getNodes().size()+1; i++){
        if(sys->getNodes()[i] != NULL){
            if(sys->getNodes()[i]->isFolder){
                node.append(sys->getNodes()[i]);
            }
        }
    }*/


}

QList<INode *> MainWindow::getFoldersInFolder(INODESYSTEM *sys, string folderName) {
    INode *folder = sys->getNodes()[sys->findFile(folderName)];

    string data;
    for (int i = 0; i < folder->blockList.size(); i++) {

        data = data + sys->getDisk()->getBlocks()[folder->blockList[i]];
    }

    vector<int> nums = splitStringIntoInts(data);

    QList < INode * > node;
    for (int i = 0; i < nums.size(); i++) {
        if (sys->getNodes()[nums[i]]->isFolder) {
            node.append(sys->getNodes()[nums[i]]);
        }
    }
    return node;
}

QList<INode *> MainWindow::getFilesInFolder(INODESYSTEM *sys, string folderName) {
    INode *folder = sys->getNodes()[sys->findFile(folderName)];

    string data;
    for (int i = 0; i < folder->blockList.size(); i++) {

        data = data + sys->getDisk()->getBlocks()[folder->blockList[i]];
    }

    vector<int> nums = splitStringIntoInts(data);

    QList < INode * > node;
    for (int i = 0; i < nums.size(); i++) {
        node.append(sys->getNodes()[nums[i]]);
    }
    return node;
}

void MainWindow::showFilesInFolder(INODESYSTEM *sys, string folderName) {
    int fileId = sys->findFile(folderName);

    if (sys->getNodes()[fileId]->isFolder) {
        createTableFileRows(getFilesInFolder(sys, folderName));
    }
}

vector<int> MainWindow::splitStringIntoInts(string inputString) {
    stringstream ss(inputString);
    string token;
    vector <string> tokens;
    vector<int> nums;
    char delimiter = ',';
    if (inputString != " ") {
        while (getline(ss, token, delimiter)) {
            if (token != " ") {
                tokens.push_back(token);
                //qDebug() << "splitStringIntoInts: token -> " << token;
                nums.push_back(stoi(token));
            }
        }
    }
    return nums;
}

void MainWindow::on_treeWidget_DiskD_itemClicked(QTreeWidgetItem *item, int column) {
    showFilesInFolder(sys, item->text(column).toStdString());
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row) {
    string folderName = ui->tableWidget->item(row, 1)->text().toStdString();
    showFilesInFolder(sys, folderName);
}


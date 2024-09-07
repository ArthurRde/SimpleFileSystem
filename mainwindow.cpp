#include "mainwindow.h"
#include "ui_dialogcreatefile.h"
#include "ui_mainwindow.h"


Disk *diskD = new Disk(64, 512);
Disk *diskC = new Disk(64, 512);
INODESYSTEM *sys = new INODESYSTEM(diskD->getSize(), diskD);
FATSYSTEM *fSys = new FATSYSTEM(diskC->getBlocks().size(),diskC->getBlockSize());

int MainWindow::getSlotSelected() const
{
    return slotSelected;
}

void MainWindow::setSlotSelected(int newSlotSelected)
{
    slotSelected = newSlotSelected;
}

INode* MainWindow::getClipboardFileNode() const
{
    return clipboardFileNode;
}

void MainWindow::setClipboardFileNode(INode *newClipboardFileNode)
{
    clipboardFileNode = newClipboardFileNode;
}

bool MainWindow::getClipboardNodeCopied() const
{
    return clipboardNodeCopied;
}

void MainWindow::setClipboardNodeCopied(bool newClipboardNodeCopied)
{
    clipboardNodeCopied = newClipboardNodeCopied;
}

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

    //fSys->createFile(512,"helloWorld.txt");

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton->setEnabled(false);
    connect(ui->actionCreate_demofiles, &QAction::triggered, this, &MainWindow::createDemoFiles);
    // Setup

    ui->treeWidget_DiskD->setColumnCount(1);
    ui->treeWidget_DiskD->setHeaderLabels(QStringList() << "Folders");
    int rootId = stoi(diskD->getBlocks().at(0));
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
QString MainWindow::findFolderPath(INODESYSTEM *sys, int rootId, string foldername) {
    QString path;
    QList < INode * > children = sys->getFoldersInFolder( sys->getNodes()[rootId]->name);
    if(children.contains(sys->getNodes()[sys->findFile(foldername)])){
        return QString::fromStdString(sys->getNodes()[rootId]->name + "/" + foldername );
    }
    for (int i = 0; i < children.size(); i++) {

        QList < INode * > newChild = sys->getFoldersInFolder( children[i]->name);

        if (newChild.size() > 0) {
            QString returnValue = findFolderPath(sys,sys->findFile(children[i]->name) , foldername);
            if(returnValue != "notFound"){

                path = QString::fromStdString(sys->getNodes()[rootId]->name)+ "/"   +returnValue;

                return path;
            }
        } else {
            return "notFound";
        }
    }
    return "notFound";
}

void MainWindow::showPath(INODESYSTEM *sys, int rootId){
    QString val = findFolderPath(sys, rootId, currentFolder);
    if(val == "notFound"){
    ui->label_3->setText("Path: ~/");
    } else {
        ui->label_3->setText("Path: "+val);
    }
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
    //qDebug() << "show all Folder " << "rec start ";
    QList < INode * > children = sys->getFoldersInFolder( sys->getNodes()[rootId]->name);
    //qDebug() << "show all Folder " << "children";
    for (int i = 0; i < children.size(); i++) {
        QTreeWidgetItem *item = setTreeWidgetChild(rootItem, children[i]->name);
        //qDebug() << "show all Folder " << "setChild";
        QList < INode * > newChild = sys->getFoldersInFolder( children[i]->name);
        //qDebug() << "show all Folder " << "newChild";
        if (newChild.size() > 0) {
            //qDebug() << "show all Folder " << "newRec";
            setTreeWidgetChildRec(sys, sys->findFile(children[i]->name), item);
           // qDebug() << "show all Folder " << "Rec finished";
        }
    }
}


void MainWindow::showAllFolder(INODESYSTEM *sys, int rootId) {
    ui->treeWidget_DiskD->clear();
    //root folder anzeigen
    //qDebug() << "show all Folder " << "clear";
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget_DiskD);
    rootItem->setText(0, QString::fromStdString(sys->getNodes()[rootId]->name));
   // qDebug() << "show all Folder " << "root";
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




void MainWindow::showFilesInFolder(INODESYSTEM *sys, string folderName) {
    int fileId = sys->findFile(folderName);

    if (sys->getNodes()[fileId]->isFolder) {
        currentFolder = folderName;
        // qDebug() << "FolderName" << currentFolder;
        createTableFileRows(sys->getFilesInFolder( folderName));
        showPath(sys, stoi(diskD->getBlocks().at(0)));
    }
}



void MainWindow::on_treeWidget_DiskD_itemClicked(QTreeWidgetItem *item, int column) {
    showFilesInFolder(sys, item->text(column).toStdString());
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row) {
    string fileName = ui->tableWidget->item(row, 1)->text().toStdString();
    showFilesInFolder(sys, fileName);
    showDataOfFile(sys, fileName);
}


void MainWindow::showDataOfFile(INODESYSTEM *sys, string fileName){
    qDebug() << "show file 1";
    int fileId = sys->findFile(fileName);
    qDebug() << "show file 2";
    if (!sys->getNodes()[fileId]->isFolder) {
        qDebug() << "show file 3";
        DialogShowFile dlg(sys->getNodes()[fileId]->name, sys->getNodes()[fileId]->author, sys->getNodes()[fileId]->date,sys->getNodes()[fileId]->size, sys->getDataOfFile(fileName));
        dlg.setWindowTitle("File information");
        if(dlg.exec() == QDialog::Accepted){
            sys->editData(fileId, dlg.getDataText());
        }

        qDebug() << "show file 4";
    }
    int rootId = stoi(diskD->getBlocks().at(0));
    showAllFolder(sys, rootId);
    showFilesInFolder(sys, currentFolder);
}
void MainWindow::on_pushButton_clicked()
{
    //TODO: add new window to write new filename into


    DialogRename dlg(ui->tableWidget->item(slotSelected, 1)->text().toStdString());
    dlg.setWindowTitle("Rename a File");
    dlg.show();

    if(dlg.exec() == QDialog::Accepted){
        sys->renameFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString(), dlg.getName());
    }

    int rootId = stoi(diskD->getBlocks().at(0));
    showAllFolder(sys,rootId);
    showFilesInFolder(sys, currentFolder);

}


void MainWindow::on_pushButton_5_clicked()
{
    DialogCreateFile dlg;
    dlg.setWindowTitle("Create a File");
    dlg.show();
    if(dlg.exec() == QDialog::Accepted) {

        if(dlg.getUi()->comboBox->currentText() == "Folder"){
            sys->createFile(dlg.getUi()->lineEdit->text().toStdString(),dlg.getUi()->lineEdit_2->text().toStdString()," ",currentFolder,true);
        } else {
            if(dlg.getUi()->comboBox->currentText() != "custom"){
                string name = dlg.getUi()->lineEdit->text().toStdString() + "." + dlg.getUi()->comboBox->currentText().toStdString();
                sys->createFile(name,dlg.getUi()->lineEdit_2->text().toStdString(),dlg.getUi()->lineEdit_3->text().toStdString(),currentFolder);
            } else {
                sys->createFile(dlg.getUi()->lineEdit->text().toStdString(),dlg.getUi()->lineEdit_2->text().toStdString(),dlg.getUi()->lineEdit_3->text().toStdString(),currentFolder);
            }
        }

    }

    int rootId = stoi(diskD->getBlocks().at(0));

    showAllFolder(sys, rootId);

    showFilesInFolder(sys, currentFolder);

}


void MainWindow::on_tableWidget_itemSelectionChanged()
{
    QItemSelectionModel *selectionModel = ui->tableWidget->selectionModel();

    if (selectionModel->hasSelection()) {
        QModelIndexList selectedRows = selectionModel->selectedRows();
        if (!selectedRows.isEmpty()) {
            // Line Selec


            ui->pushButton_2->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton->setEnabled(true);
            slotSelected = selectedRows[0].row();
        } else {
            // No line selected
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton->setEnabled(false);
        }
    } else {
        // No line sel
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    sys->deleteFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString());
    showFilesInFolder(sys, currentFolder);
    showAllFolder(sys, stoi(diskD->getBlocks().at(0)));
}


void MainWindow::on_pushButton_3_clicked()
{

    qDebug() << "file";
    ui->pushButton_4->setEnabled(true);
    clipboardNodeCopied = true;
    clipboardFileNode = sys->getNodes()[sys->findFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString())];
    qDebug() << "Coppied File" << clipboardFileNode->name;
}


void MainWindow::on_pushButton_4_clicked()
{

    sys->copyFile(clipboardFileNode->name,currentFolder);
    showFilesInFolder(sys, currentFolder);
    showAllFolder(sys, stoi(diskD->getBlocks().at(0)));
}

void MainWindow::createDemoFiles(){
    int rootId = stoi(diskD->getBlocks().at(0));

    sys->createFile("documents", "user", " ", "root", true);

    sys->createFile("downloads", "system", " ", "root", true);

    sys->createFile("keyAccess.closed", "KeyCode.exe", "bearer237746643698892388734794378478979847942794279842978974821", "root");

    sys->createFile("poem.txt", "author", "Betriebssysteme, still und leise,Lenken uns auf ihre Weise.Windows, Linux, macOS,Jedes hat sein eigenes SOS.Sie starten Programme, verwalten Dateien,Lassen uns in digitale Welten eintauchen und verweilen.Mit Klicks und Tasten, so flink und schnell,Machen sie unser Leben digital und hell.Doch manchmal, oh, da gibt’s Probleme,Ein Absturz, ein Fehler, das sind die Themen.Doch wir wissen, sie sind stets bereit,Für uns zu arbeiten, Tag und Nacht, jederzeit.", "documents");

    sys->createFile("examSolutions.pdf", "anonymus", "Frage: Erklären Sie das OSI-Modell und nennen Sie die sieben Schichten.solution: Das OSI-Modell (Open Systems Interconnection Model) ist ein Referenzmodell für die Kommunikation in Netzwerken. Es besteht aus sieben Schichten:Physikalische Schicht (Physical Layer)Sicherungsschicht (Data Link Layer)Netzwerkschicht (Network Layer)Transportschicht (Transport Layer)Sitzungsschicht (Session Layer)Darstellungsschicht (Presentation Layer)Anwendungsschicht (Application Layer)", "downloads");

    sys->createFile("photos", "system", " ", "documents", true);

    sys->createFile("summer.png", "apple", "A beautiful summer, A beautiful summer", "photos");

    qDebug() << "demofiles created";

    showAllFolder(sys, rootId);
    showFilesInFolder(sys, "root");
}

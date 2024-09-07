#include "mainwindow.h"
#include "ui_dialogcreatefile.h"
#include "ui_mainwindow.h"

using namespace std;
Disk *diskD = new Disk(64, 512);
Disk *diskC = new Disk(64, 512);
INODESYSTEM *Isys = new INODESYSTEM(diskD->getSize(), diskD);
FATSYSTEM *fSys = new FATSYSTEM(diskC->getSize(), diskC);

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
    showAllFolder(Isys, rootId);
    showAllFolder(fSys,"root");
    showFilesInFolder(Isys, "root");
    showFilesInFolder(fSys,"root");

    //fSys->createFile("me.mp4", "tets", " ", " ");
    //fSys->createFile("file.pdf", "dud", "jkfjnjfsnjsjndsjnjnjndjndsjnd", " ");
    //showFilesInFolder(fSys, " ");
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



QString MainWindow::findFolderPath(FATSYSTEM *sys, char* rootName, char* foldername) {
    QString path;
    //qDebug() << "start show Path fat 1" << foldername;
    QList < File * > children = sys->getFoldersInFolder( rootName);
    //qDebug() << "start show Path fat 2" << children.size();
    if(children.contains(sys->findFile(foldername))){
        string str;
        str.assign(rootName);
         //qDebug() << "start show Path fat ret";
        return QString::fromStdString(str + "/" + foldername );

    }
    for (int i = 0; i < children.size(); i++) {
        // qDebug() << "start show Path fat 3";
        QList < File * > newChild = sys->getFoldersInFolder( children[i]->name);

        if (newChild.size() > 0) {
          //   qDebug() << "start show Path fat 4";
            QString returnValue = findFolderPath(sys,children[i]->name , foldername);
            if(returnValue != "notFound"){
                string str;
                str.assign(rootName);
                path = QString::fromStdString(str+ "/"   )+ returnValue;

                return path;
            }
        } else {
           // qDebug() << "not Found ";
            return "notFound";
        }
    }
     //qDebug() << "not Found";
    return "notFound";
}

void MainWindow::showPath(FATSYSTEM *sys, char* rootName){
    char c[currentFolder.length() + 1];
    char* c_folder = c;
    strcpy(c_folder, currentFolder.c_str());
    //qDebug() << "start show Path fat" ;
    QString val = findFolderPath(sys, rootName, c_folder);
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

void MainWindow::createTableFileRows(QList<File*> files) {
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(files.size());
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Icon" << "Name" << "Size" << "Date");
    for (int i = 0; i < files.size(); i++) {

            QTableWidgetItem *iconItem = new QTableWidgetItem;
            iconItem->setIcon(QIcon("path/to/icon.png"));
            iconItem->setFlags(iconItem->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 0, iconItem);

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
           // qDebug() << sys->getNodes()[i]->name;
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
    qDebug() << "show all Folder " << "clear";
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget_DiskD);
    rootItem->setText(0, QString::fromStdString(sys->getNodes()[rootId]->name));
    qDebug() << "show all Folder " << "root";
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




void MainWindow::setTreeWidgetChildRec(FATSYSTEM *sys, char* rootName, QTreeWidgetItem *rootItem) {
  //  qDebug() << "show all Folder " << "rec start ";
    QList < File*> children = sys->getFoldersInFolder( sys->findFile(rootName)->name);
    //qDebug() << "show all Folder " << "children" << children.size();
    for (int i = 0; i < children.size(); i++) {
        QTreeWidgetItem *item = setTreeWidgetChild(rootItem, children[i]->name);
      //  qDebug() << "show all Folder " << "setChild";
        QList < File * > newChild = sys->getFoldersInFolder( children[i]->name);
        //qDebug() << "show all Folder " << "newChild";
        if (newChild.size() > 0) {
          //  qDebug() << "show all Folder " << "newRec";
            setTreeWidgetChildRec(sys, children[i]->name, item);
            //qDebug() << "show all Folder " << "Rec finished";
        }
    }
}


void MainWindow::showAllFolder(FATSYSTEM *sys, char* rootName) {
    ui->treeWidget_DiskC->clear();
    //root folder anzeigen
    //qDebug() << "show all Folder " << "clear";
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget_DiskC);
    rootItem->setText(0, QString::fromStdString(sys->findFile("root")->name));
    // qDebug() << "show all Folder " << "root";
    setTreeWidgetChildRec(sys, rootName, rootItem);


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
        showedSystem = 1;
    }
}

void MainWindow::showFilesInFolder(FATSYSTEM *sys, string folderName) {
    if(sys->findFile(folderName.c_str())->isFolder){
    currentFolder = folderName;
    QList <File> files;
    char c[folderName.length() + 1];
    char* c_folder = c;
    strcpy(c_folder, folderName.c_str());
    qDebug() << "showFiles" << c_folder;

    showPath(sys,"root");
    createTableFileRows(sys->getFilesInFolder(c_folder));
    showedSystem = 2;
    }
}



void MainWindow::on_treeWidget_DiskD_itemClicked(QTreeWidgetItem *item, int column) {
    showFilesInFolder(Isys, item->text(column).toStdString());
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row) {
    string fileName = ui->tableWidget->item(row, 1)->text().toStdString();
    if(showedSystem == 1){
    showFilesInFolder(Isys, fileName);
    showDataOfFile(Isys, fileName);
    } else {
    showFilesInFolder(fSys,fileName);


    }
}


void MainWindow::showDataOfFile(INODESYSTEM *sys, string fileName){
    //qDebug() << "show file 1";
    int fileId = sys->findFile(fileName);
   // qDebug() << "show file 2";
    if (!sys->getNodes()[fileId]->isFolder) {
       // qDebug() << "show file 3";
        DialogShowFile dlg(sys->getNodes()[fileId]->name, sys->getNodes()[fileId]->author, sys->getNodes()[fileId]->date,sys->getNodes()[fileId]->size, sys->getDataOfFile(fileName));
        dlg.setWindowTitle("File information");
        if(dlg.exec() == QDialog::Accepted){
            sys->editData(fileId, dlg.getDataText());
        }

      //  qDebug() << "show file 4";
    }
    int rootId = stoi(diskD->getBlocks().at(0));
    showAllFolder(sys, rootId);
    showFilesInFolder(sys, currentFolder);
}

char* stringToCharArray(const string& str) {

    char* char_array = new char[str.length() + 1];


    strcpy(char_array, str.c_str());
    return char_array;
}


void MainWindow::on_pushButton_clicked()
{


    DialogRename dlg(ui->tableWidget->item(slotSelected, 1)->text().toStdString());
    dlg.setWindowTitle("Rename a File");
    dlg.show();

    if(dlg.exec() == QDialog::Accepted){
        if(showedSystem == 1){
        Isys->renameFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString(), dlg.getName());
             int rootId = stoi(diskD->getBlocks().at(0));
            showAllFolder(Isys,rootId);
            showFilesInFolder(Isys, currentFolder);
        } else {
            fSys->renameFile(stringToCharArray(currentFolder),stringToCharArray(ui->tableWidget->item(slotSelected, 1)->text().toStdString()), stringToCharArray(dlg.getName() ));
            showAllFolder(fSys,"root");
            showFilesInFolder(fSys,currentFolder);
        }
    }




}




void MainWindow::on_pushButton_5_clicked()
{
    DialogCreateFile dlg;
    dlg.setWindowTitle("Create a File");
    dlg.show();
    if(dlg.exec() == QDialog::Accepted) {
        if(showedSystem == 1){
        if(dlg.getUi()->comboBox->currentText() == "Folder"){
            Isys->createFile(dlg.getUi()->lineEdit->text().toStdString(),dlg.getUi()->lineEdit_2->text().toStdString()," ",currentFolder,true);
        } else {
            if(dlg.getUi()->comboBox->currentText() != "custom"){
                string name = dlg.getUi()->lineEdit->text().toStdString() + "." + dlg.getUi()->comboBox->currentText().toStdString();
                Isys->createFile(name,dlg.getUi()->lineEdit_2->text().toStdString(),dlg.getUi()->lineEdit_3->text().toStdString(),currentFolder);
            } else {
                Isys->createFile(dlg.getUi()->lineEdit->text().toStdString(),dlg.getUi()->lineEdit_2->text().toStdString(),dlg.getUi()->lineEdit_3->text().toStdString(),currentFolder);
            }
        }

        int rootId = stoi(diskD->getBlocks().at(0));

        showAllFolder(Isys, rootId);

        showFilesInFolder(Isys, currentFolder);
        } else {
            qDebug() << "Fat Create UI";
            if(dlg.getUi()->comboBox->currentText() == "Folder"){
            qDebug() << "Fat Create UI 2";

                char* c_name = stringToCharArray(dlg.getUi()->lineEdit->text().toStdString());



                char* c_author = stringToCharArray(dlg.getUi()->lineEdit_2->text().toStdString());



                char* c_folder = stringToCharArray(currentFolder);

                qDebug() << "Fat Create UI" << c_name << " " << c_author << " " << c_folder;
                fSys->createFile(c_name,c_author," ",c_folder,true);
            } else {
                if(dlg.getUi()->comboBox->currentText() != "custom"){

                    char cN[currentFolder.length() + 1];
                    char* c_name = cN;
                    strcpy(c_name, (dlg.getUi()->lineEdit->text().toStdString()+"."+dlg.getUi()->comboBox->currentText().toStdString()).c_str());

                    char cA[currentFolder.length() + 1];
                    char* c_author = cA;
                    strcpy(c_name,dlg.getUi()->lineEdit_2->text().toStdString().c_str());

                    char c[currentFolder.length() + 1];
                    char* c_folder = c;
                    strcpy(c_folder, currentFolder.c_str());


                   fSys->createFile(c_name,c_author,dlg.getUi()->lineEdit_3->text().toStdString(),c_folder);
                } else {

                    char cN[currentFolder.length() + 1];
                    char* c_name = cN;
                    strcpy(c_name, dlg.getUi()->lineEdit->text().toStdString().c_str());

                    char cA[currentFolder.length() + 1];
                    char* c_author = cA;
                    strcpy(c_name,dlg.getUi()->lineEdit_2->text().toStdString().c_str());

                    char c[currentFolder.length() + 1];
                    char* c_folder = c;
                    strcpy(c_folder, currentFolder.c_str());

                    fSys->createFile(c_name,c_author,dlg.getUi()->lineEdit_3->text().toStdString(),c_folder);
                }
            }

            showAllFolder(fSys, "root");

            showFilesInFolder(fSys, currentFolder);
        }

    }



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
    if(showedSystem == 1){
    Isys->deleteFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString());
    showFilesInFolder(Isys, currentFolder);
    showAllFolder(Isys, stoi(diskD->getBlocks().at(0)));
    } else {
        qDebug() << "delete conf 0" ;
        fSys->deleteFile( stringToCharArray(ui->tableWidget->item(slotSelected, 1)->text().toStdString()));
        qDebug() << "delete conf 1" ;
        showFilesInFolder(fSys,stringToCharArray(currentFolder));
        qDebug() << "delete conf 2" ;
        showAllFolder(fSys, "root");
        qDebug() << "delete conf 3" ;
    }
}


void MainWindow::on_pushButton_3_clicked()
{

   // qDebug() << "file";
    ui->pushButton_4->setEnabled(true);
   if(showedSystem == 1){
    clipboardNodeCopied = true;
    clipboardFatCopied = false;
    clipboardFileFat = NULL;
    clipboardFileNode = Isys->getNodes()[Isys->findFile(ui->tableWidget->item(slotSelected, 1)->text().toStdString())];
   } else {
       clipboardNodeCopied = false;
       clipboardFatCopied = true;
       clipboardFileNode = NULL;
       clipboardFileFat = fSys->findFile(stringToCharArray(ui->tableWidget->item(slotSelected, 1)->text().toStdString()));
   }
    //qDebug() << "Coppied File" << clipboardFileNode->name;
}


void MainWindow::on_pushButton_4_clicked()
{
    if(false){
    Isys->copyFile(clipboardFileNode->name,currentFolder);
    showFilesInFolder(Isys, currentFolder);
    showAllFolder(Isys, stoi(diskD->getBlocks().at(0)));
    } else {
        fSys->copyFile(clipboardFileFat->name,stringToCharArray(currentFolder));
        showFilesInFolder(fSys,currentFolder);
        showAllFolder(fSys,"root");
    }
}

void MainWindow::createDemoFiles(){
    int rootId = stoi(diskD->getBlocks().at(0));

    Isys->createFile("documents", "user", " ", "root", true);

    Isys->createFile("downloads", "system", " ", "root", true);

    Isys->createFile("keyAccess.closed", "KeyCode.exe", "bearer237746643698892388734794378478979847942794279842978974821", "root");

    Isys->createFile("poem.txt", "author", "Betriebssysteme, still und leise,Lenken uns auf ihre Weise.Windows, Linux, macOS,Jedes hat sein eigenes SOS.Sie starten Programme, verwalten Dateien,Lassen uns in digitale Welten eintauchen und verweilen.Mit Klicks und Tasten, so flink und schnell,Machen sie unser Leben digital und hell.Doch manchmal, oh, da gibt’s Probleme,Ein Absturz, ein Fehler, das sind die Themen.Doch wir wissen, sie sind stets bereit,Für uns zu arbeiten, Tag und Nacht, jederzeit.", "documents");

    Isys->createFile("examSolutions.pdf", "anonymus", "Frage: Erklären Sie das OSI-Modell und nennen Sie die sieben Schichten.solution: Das OSI-Modell (Open Systems Interconnection Model) ist ein Referenzmodell für die Kommunikation in Netzwerken. Es besteht aus sieben Schichten:Physikalische Schicht (Physical Layer)Sicherungsschicht (Data Link Layer)Netzwerkschicht (Network Layer)Transportschicht (Transport Layer)Sitzungsschicht (Session Layer)Darstellungsschicht (Presentation Layer)Anwendungsschicht (Application Layer)", "downloads");

    Isys->createFile("photos", "system", " ", "documents", true);

    Isys->createFile("summer.png", "apple", "A beautiful summer, A beautiful summer", "photos");

    qDebug() << "demofiles created";

    showAllFolder(Isys, rootId);
    showFilesInFolder(Isys, "root");

    fSys->createFile("documents", "user", " ", "root", true);
    fSys->createFile("hello.txt", "sys", "data", "root");
    fSys->createFile("photos", "sys", " ", "documents",true);
    showFilesInFolder(fSys, "root");
    showAllFolder(fSys,"root");

}

void MainWindow::on_treeWidget_DiskC_itemClicked(QTreeWidgetItem *item, int column)
{
    showFilesInFolder(fSys, item->text(column).toStdString());
}


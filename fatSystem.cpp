#include "fatSystem.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

Disk* FATSYSTEM::getDisk() const
{
    return disk;
}

void FATSYSTEM::setDisk( Disk *newDisk)
{
    disk = newDisk;
}

Fat *FATSYSTEM::getFat() const
{
    return fat;
}

void FATSYSTEM::setFat(Fat *newFat)
{
    fat = newFat;
}

FATSYSTEM::FATSYSTEM(int diskSpace, Disk *disk_) {
    Fat *pFat = new Fat;
    disk = disk_;

    pFat->totalBlocks = diskSpace / disk_->getBlockSize();
    pFat->blockSize = disk_->getBlockSize();
    for (int i = 0; i < pFat->totalBlocks; i++) {
        qDebug() << "hereee";
        pFat->blockStatus.push_back(BLOCK_FREE);
        disk->getBlocks()[i] = " ";
    }

    fat = pFat;
    createFile("root", "system", " ", "isRoot", true);

}

FATSYSTEM::~FATSYSTEM(){
    delete fat;
}

Cluster *FATSYSTEM::createCluster(int blockIndex) {
    auto *cluster = new Cluster;
    cluster->blockIndex = blockIndex;
    cluster->next = nullptr;
    cluster->prev = nullptr;
    return cluster;
}

int FATSYSTEM::getFreeDiskSpace() {
    int freeBlocks = 0;
    for (int i = 0; i < fat->totalBlocks; i++) {
        if (fat->blockStatus[i] == BLOCK_FREE) {
            freeBlocks++;
        }
    }
    return freeBlocks * fat->blockSize;
}

File *FATSYSTEM::createFile(char* name_, char* author_, string data,char* parentName, bool isFolder) {
    //showFat();
    qDebug() << "free disk" << getFreeDiskSpace() << name_;
   /** if (strlen(name_) > 31) {
        cout << "Name zu lang" << endl;
        return nullptr;
    }*/
   qDebug() << "create File 1" << name_;
    vector<string> dataChunks;
    size_t chunk_size = disk->getBlockSize();
    size_t length = data.length();
    int requiredBlocks = 0;
    //qDebug() << "create File 2";
    for (size_t i = 0; i < length; i += chunk_size) {
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }

    qDebug() << "create File 3";
    if (requiredBlocks > getFreeDiskSpace() / fat->blockSize) {
        cout << "Nicht genug Platz" << endl;
        return nullptr;
    }


    File *newFile = new File;
    strncpy(newFile->name, name_, 31);
    if(strcmp(parentName, "isRoot")!= 0){
        if(existFile(name_)){
            qDebug() << "create unique name";
            strncpy(newFile->name, createUniqueName(name_,"-D").c_str(), 31);
        }
    }

    newFile->name[31] = '\0';
    qDebug() << "create unique name" << newFile->name;
    newFile->size = length;
    newFile->clusterList = nullptr;
    newFile->isFolder = isFolder;
    newFile->date = QDateTime::currentDateTime();
    //qDebug() << "create File 4";
    string parentData = "";
    int result = 0;
    if(parentName != "isRoot"){
        result = addFileToFolder(parentName, name_);

    }
    //qDebug() << "create File 5";
    if(result == 0){ // error handling
        //schauen ob genug platz da


        if (requiredBlocks > getFreeDiskSpace() / fat->blockSize) {
            editData(parentName, parentData);
     //       qDebug() <<"Nicht genug Platz" ;
            return NULL;
        }
    }


   // qDebug() << "create File 6";

    Cluster *lastCluster = nullptr;
    int allocatedBlocks = 0;
    int pos = 0;
    for (int i = 0; i < fat->totalBlocks && allocatedBlocks < requiredBlocks; i++) {
        if (fat->blockStatus[i] == BLOCK_FREE) {
            fat->blockStatus[i] = BLOCK_OCCUPIED;
          //    qDebug() << "create File 6.1";
            Cluster *cluster = createCluster(i);
            disk->addDataToBlock(i, dataChunks[pos]);
            pos++;
        //      qDebug() << "create File 6.2";
            if (newFile->clusterList == nullptr) {
                newFile->clusterList = cluster;
            } else {
                lastCluster->next = cluster;
                cluster->prev = lastCluster;
            }
            //qDebug() << "create File 6.3";
           // qDebug() << "Cluster" << cluster->blockIndex;

            lastCluster = cluster;
            allocatedBlocks++;
        }
    }
    fat->files.push_back(newFile);

    qDebug() << "create File 7" << newFile->name;


    return newFile;
}

void FATSYSTEM::showCluster(char* fileName){

   // qDebug() << "showCluster";

    Cluster *cluster = findFile(fileName)->clusterList;
    //qDebug() << "showCluster 3";
    while (cluster != nullptr) {
      //  qDebug() << "showCluster 3";
        //qDebug() << "show Cluster " << cluster->blockIndex;
        cluster = cluster->next;

    }
}

string FATSYSTEM::getDataOfFile(char* fileName){

    string data = "";
    File *fileToRead= findFile(fileName);
    Cluster *cluster = fileToRead->clusterList;

    while (cluster != nullptr) {
        data = data +disk->getBlocks()[cluster->blockIndex];

        cluster = cluster->next;

    }
    //qDebug() << "data Found" << data;
    return data;
}

QList<File*> FATSYSTEM::getFilesInFolder(char* folderName) {
    QList<File*> filesInFolder;
    //qDebug() << "files in " <<folderName;

    string data = getDataOfFile(folderName);

    //qDebug() << "files in 1" << data;
    vector<string> names = splitStringIntoParts(data);
    //qDebug() << "files in 2" << names.size() ;

    QList < File * > files;
    for (int i = 0; i < names.size(); i++) {

        filesInFolder.append(findFile(names[i].c_str()));
      //  qDebug() << "findFile";
    }
     //qDebug() << "files in 3" ;
    return filesInFolder;
}

string FATSYSTEM::createUniqueName(char* fileName, char* uniqueText){
   // qDebug() << "test 1" << fileName;
    char* nameChar = NULL;
    bool ex = existFile(fileName);
    string str = "";
   // qDebug() << "test 1.1";
    while(ex){


        stringstream ss(fileName);
        string token;
        vector <string> tokens;
        //qDebug() << "test 1.2";
        char delimiter = '.';
        if (str != " ") {
            while (getline(ss, token, delimiter)) {
                if ((token != " ")) {
                    if(!token.empty()){
                        tokens.push_back(token);

                    }
                }
            }
        }
       // qDebug() << "test 2";
        tokens[0] = tokens[0]+uniqueText;



        str = "";

        str = str + tokens[0].c_str();
        for(int i = 1; i< tokens.size(); i++){

            str = str +"."+tokens[i];
        }
       // qDebug() << "here" << str;

        //qDebug() << "new Name " << nameChar <<fat->files.size() ;


        ex = existFile(const_cast<char*>(str.c_str()));
    }
    //qDebug() << "test 3";
    return str.c_str();
}

void FATSYSTEM::copyFile(char* fileName, char* folderName){

    char* name = const_cast<char*>(createUniqueName(fileName).c_str());

    if(!findFile(fileName)->isFolder){
        bool ex = existFile(findFile(fileName)->name);
        createFile(name,findFile(fileName)->author,getDataOfFile(findFile(fileName)->name),folderName);

    } else{
       // qDebug() << "folder paste";
        createFile(name,findFile(fileName)->author," ",folderName,true);


        vector<string> names = splitStringIntoParts(getDataOfFile(fileName));
        editData(fileName, " ");
        //neuen nummers
        for(int i = 0; i < names.size(); i++ ){


            copyFile(findFile(names[i].c_str())->name,name);
        }


    }
}

bool FATSYSTEM::existFile(char* fileName){
    bool ex = false;

    for(int i = 0; i < fat->files.size(); i++){

        if(fat->files[i] != NULL){
           // qDebug() << "test if exist" << fileName << " mit " << fat->files[i]->name << strcmp(fileName , fat->files[i]->name);
            if(strcmp(fileName , fat->files[i]->name) == 0){
                qDebug() << "exist " << fileName;
                ex = true;
            }
        }

    }

    return ex;
}

char* FATSYSTEM::getFolderOfFile(char* fileName){

    for(int i = 0; i < fat->files.size(); i++){
        if(fat->files[i] != NULL){
            if(fat->files[i]->isFolder){
                if(getFilesInFolder(fat->files[i]->name).contains(findFile(fileName))){
                    return fat->files[i]->name;
                }
            }
        }
    }
    return " ";
}

vector<string> FATSYSTEM::splitStringIntoParts(string inputString){
    stringstream ss(inputString);
    //qDebug() << "split" << inputString;
    string token;
    vector <string> tokens;


    char delimiter = ',';
    if (inputString != " ") {
        while (getline(ss, token, delimiter)) {
            if ((token != " ")) {
                if(!token.empty()){
      //              qDebug() << "splitt" << token;
                    tokens.push_back(token);
                    ;
                    //qDebug() << "splitStringIntoInts: token -> " << token;

                }
            }
        }
    }
    return tokens;
}
File* FATSYSTEM::findFile(char* fileName){
   // qDebug() << "return 0 ";
    for(int i = 0; i < fat->files.size(); i++){
     //   qDebug() << "return 1 ";
        if(fat->files[i] != nullptr){
       //     qDebug() << "return 2 " << fat->files[i]->name;
            if(strcmp(fileName , fat->files[i]->name) == 0){
         //   qDebug() << "return 3";
           return fat->files[i];
        }
        }
    }
    return NULL;
}

File* FATSYSTEM::findFile(const char* fileName){
   // qDebug() << "return 0 ";
    for(int i = 0; i < fat->files.size(); i++){
     //   qDebug() << "return 1 ";
        if(fat->files[i] != nullptr){
       //     qDebug() << "return 2 " << fat->files[i]->name;
            if(strcmp(fileName , fat->files[i]->name) == 0){
         //       qDebug() << "return 3";
                return fat->files[i];
            }
        }
    }
    return NULL;
}

int FATSYSTEM::editData(char* fileName ,string data){
    //qDebug() << "Edit Data 1" << data;
    File* file = findFile(fileName);
    bool isEmpty = false;
    if(data.empty()){
        isEmpty = true;
    }
    //qDebug() << "Edit Data 2";
    vector<string> dataChunks;
    size_t chunk_size = fat->blockSize;
    size_t length = data.length();
    int requiredBlocks = 0;



 //qDebug() << "Edit Data 3";
    for (size_t i = 0; i < length; i += chunk_size) {
        // qDebug() << "test for";
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }

    //qDebug() << "Edit Data 4";
    //qDebug() << "test 2" << requiredBlocks << "schon " << nodes[fileToBeEditedId]->blockList.size();
    getNumberOfBlocks(fileName);
    //qDebug() << "Edit Data 4.11";
    if(getNumberOfBlocks(fileName) < requiredBlocks){

        if (requiredBlocks > getFreeDiskSpace() / fat->blockSize) {
            qDebug() <<"Nicht genug Platz" ;
            return -1;

        }
       // qDebug() << "test 3";
        file->size = length;
        requiredBlocks = requiredBlocks - getNumberOfBlocks(fileName) ;
        //qDebug() << "Edit Data 4.1";
        int allocatedBlocks = 0;
        int pos = 0;
        Cluster* lastCluster = getLastClusterOfFile(fileName);
       // qDebug() << "Edit Data 4.2";
        for (int i = 0; i < fat->totalBlocks && allocatedBlocks < requiredBlocks; i++) {
            if(fat->blockStatus[i] != NULL){
            if (fat->blockStatus[i] == BLOCK_FREE) {
                //qDebug() << "Edit Data 4.3";
                fat->blockStatus[i] = BLOCK_OCCUPIED;
                //qDebug() << "Edit Data 4.4";
                allocatedBlocks++;

                Cluster *cluster = createCluster(i);
                //qDebug() << "Edit Data 4.5";
                if (file->clusterList == nullptr) {
                    file->clusterList = cluster;
                } else {
                    lastCluster->next = cluster;
                    cluster->prev = lastCluster;
                }
               // qDebug() << "Edit Data 4.6";
                lastCluster = cluster;


            }
            }
        }
    }
    //qDebug() << "Edit Data 5";
    //qDebug() << "test 4";

    Cluster *cluster = file->clusterList;

    for (int i = 0; cluster != nullptr; i++) {

        if(!isEmpty){
      //      qDebug() << "full";
            disk->addDataToBlock(cluster->blockIndex, dataChunks[i]);
        } else {
        //    qDebug() << "empty";
           disk->addDataToBlock(cluster->blockIndex, " ");
        }

        cluster = cluster->next;


    }
   //qDebug() << "Edit Data 6";

    //qDebug() << "test 6";
    return 0;
}

int FATSYSTEM::getNumberOfBlocks(char* fileName){
    File *file = findFile(fileName);

    Cluster *cluster = file->clusterList;

    int blocks = 0;
    while (cluster != nullptr) {

        blocks++;
        cluster = cluster->next;

    }

    return blocks;

}

void FATSYSTEM::renameFile(char* folder, char* fileName, char* newName)
{

    strcpy(findFile(fileName)->name, newName);
    removeFileFromFolder(fileName, folder);
    addFileToFolder(folder, newName);
}

int FATSYSTEM::addFileToFolder(char* parentName, char* name_){
    int result;
    string parentData;
    File *fileToRead= findFile(parentName);
    Cluster *cluster = fileToRead->clusterList;

    while (cluster != nullptr) {

        parentData = parentData +disk->getBlocks()[cluster->blockIndex];
        //   qDebug() << "load parent data" << parentData;
        cluster = cluster->next;

    }
    //  qDebug() << "create File with parent" << parentData;

    if(parentData.empty()){

        result = editData(parentName, name_);

    }else {

        string dataString = parentData+","+name_;

        result = editData(parentName, dataString);

    }
    return result;
}


void FATSYSTEM::removeFileFromFolder(char* fileName, char* folderName){

    File* folder = findFile(folderName);
    vector<string> names = splitStringIntoParts(getDataOfFile(folderName));

    string newData = "";
    for(int i = 0; i < names.size(); i++){
        char* char_array = new char[names[i].length() + 1];


        strcpy(char_array, names[i].c_str());
        if(strcmp(fileName, char_array) != 0){
            newData = newData + names[i];
        }
        delete[] char_array;
    }
    editData(folderName, newData);
}
char* FATSYSTEM::findFolderOfFile(char* fileName){
    File* file = findFile(fileName);
    for(int i = 0; i < fat->files.size(); i++){
        if(fat->files[i] != NULL){
            if(fat->files[i]->isFolder){
                if(getFilesInFolder(fat->files[i]->name).contains(file)){
                    return fat->files[i]->name;
                }
            }
        }
    }

    return "";

}

Cluster* FATSYSTEM::getLastClusterOfFile(char* fileName){
    File *file = findFile(fileName);
    Cluster *cluster = file->clusterList;
    int blocks = 0;
    Cluster* lastCluster;
    while (cluster != nullptr) {

        blocks++;
        lastCluster = cluster;
        cluster = cluster->next;

    }
    return lastCluster;
}

QList<File*> FATSYSTEM::getFoldersInFolder( char* folderName) {
  //  qDebug() << "folFol " << "start " << folderName;


    // qDebug() << "folfol sy s id" << sys->findFile(folderName);
    File *folder = findFile(folderName);
   // qDebug() << "folFol " << "folder inode" << folder->name;
    string data;


    string folderData = getDataOfFile(folderName);
    vector<string> parts = splitStringIntoParts(folderData);
    QList < File * > foldersIn;
    for (int i = 0; i < parts.size(); i++) {

        if (findFile(parts[i].c_str())->isFolder) {
            foldersIn.append(findFile(parts[i].c_str()));
        }
    }
    return foldersIn;
}



void FATSYSTEM::deleteFile(const char *fileName ,bool deleteFolderInFolder,bool ignoreFolderTyp ) {
   // qDebug() << "delete 1" << fileName;
    for (int i = 0; i < fat->files.size(); i++) {
        //qDebug() << "delete 2";
        if (fat->files[i] != nullptr){
            if(strcmp(fat->files[i]->name, fileName) == 0){

                if((!fat->files[i]->isFolder) || ignoreFolderTyp){

            File *fileToDelete = fat->files[i];
            Cluster *cluster = fileToDelete->clusterList;
          //  qDebug() << "delete 3";
            while (cluster != nullptr) {
                fat->blockStatus[cluster->blockIndex] = BLOCK_FREE;
                disk->addDataToBlock(cluster->blockIndex, " ");
                Cluster *clusterToBeDeleted = cluster;
                cluster = cluster->next;
                free(clusterToBeDeleted);
            }
            //qDebug() << "delete 4";
            char* char_array = const_cast<char*>(fileName);



            //qDebug() << "delete 5";
            removeFileFromFolder(char_array,getFolderOfFile(char_array));
           // qDebug() << "delete 6";
            delete fileToDelete;
            //qDebug() << "delete 7";
            fat->files[i] = nullptr;
            //qDebug() << "delete 8";
            return;

                } else {


                    if(!deleteFolderInFolder){
                      //  qDebug() << "delete 3.2" << fileName;
                        QMessageBox msgBox;
                        msgBox.setIcon(QMessageBox::Critical);

                        msgBox.setWindowTitle("Attention");
                        msgBox.setText("You try to delete a folder! Do you want to continue?");
                        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

                        if (msgBox.exec() == QMessageBox::Yes) {
                            //yes
                            //qDebug() << "yes";
                            deleteFolder(fileName);

                        }

                    } else {
                        qDebug() << "delete Folder act";
                        deleteFolder(fileName);
                    }

                }

            }
        }
    }
    cout << "Datei nicht gefunden" << endl;
}

void FATSYSTEM::deleteFolder(const char* fileName){
    char* ncFileName = const_cast<char*>(fileName);
    qDebug() << "size" << ncFileName;
    QList<File *> file = getFilesInFolder(ncFileName);
    qDebug() << "size" << file.size();
    for(int i = 0; i < file.size(); i++){
        qDebug() << "folder " << file[i]->name;
        if(!file[i]->isFolder){
            deleteFile(file[i]->name);
        } else {
            deleteFile(file[i]->name, true);
        }
    }
    deleteFile(fileName, false, true);
}

void FATSYSTEM::showFat() {
    //qDebug() << "showFat 1";
    for (int i = 0; i < fat->totalBlocks; i++) {
        //qDebug() << "showFat 2";
        if (fat->blockStatus.at(i) == BLOCK_RESERVED) {

            cout << "R";
        } else if (fat->blockStatus.at(i) == BLOCK_DEFECT){

            cout << "D";

        } else if(fat->blockStatus[i] == BLOCK_FREE){
            cout << "F";

        } else if(fat->blockStatus[i]== BLOCK_OCCUPIED){
            //cout << "O";
            for (int j = 0; j < fat->files.size(); j++) {
                if (fat->files[j] != nullptr) {
                    Cluster *cluster = fat->files[j]->clusterList;
                    while (cluster != nullptr) {
                        if (cluster->blockIndex == i) {
                            cout << j;
                            break;
                        }
                        cluster = cluster->next;
                    }
                }
            }

        }
        cout << " ";
    }
    cout << endl;
}

float FATSYSTEM::getFragmentation() {
    int fragmentedBlocks = 0;

    for (int i = 0; i < fat->totalBlocks - 1; i++) {
        if (fat->blockStatus[i] == BLOCK_OCCUPIED){
            if (fat->blockStatus[i + 1] == BLOCK_FREE) {
                fragmentedBlocks++;
            } else if (fat->blockStatus[i + 1] == BLOCK_OCCUPIED) {
                //qDebug() << "getFrag(in for - Block Occupied - Next Occupied)" << "iteration: " << i <<"Current blockStatus: " << fat->blockStatus[i];
                int fileNum = 0;
                for (int j = 0; j < fat->files.size() ; j++) {
                    if (fat->files[j] != nullptr) {
                        Cluster *cluster = fat->files[j]->clusterList;
                        while (cluster != nullptr) {
                            if (cluster->blockIndex == i) {
                                fileNum = j;
                                break;
                            }
                            cluster = cluster->next;
                        }
                    }
                }
                //qDebug() << "getFrag(in for - Block Reserved/Defect)" << "iteration: " << i <<"Current blockStatus: " << fat->blockStatus[i];
                int nextFileNum = 0;
                for (int j = 0; j < fat->files.size(); j++) {
                    if (fat->files[j] != nullptr) {
                        Cluster *cluster = fat->files[j]->clusterList;
                        while (cluster != nullptr) {
                            if (cluster->blockIndex == i + 1) {
                                nextFileNum = j;
                                break;
                            }
                            cluster = cluster->next;
                        }
                    }
                }
                if (nextFileNum != fileNum) {
                    fragmentedBlocks++;

                }

            }


            if (fat->blockStatus[i] == BLOCK_FREE) {
                if (fat->blockStatus[i + 1] == BLOCK_OCCUPIED) {
                    fragmentedBlocks++;
                }
            }
        }


        if ((fat->blockStatus[fat->totalBlocks - 2] == BLOCK_OCCUPIED &&
             fat->blockStatus[fat->totalBlocks - 1] == BLOCK_FREE) ||
            (fat->blockStatus[fat->totalBlocks - 1] == BLOCK_OCCUPIED &&
             fat->blockStatus[fat->totalBlocks - 2] == BLOCK_FREE)) {

            fragmentedBlocks++;
        } else {
            int fileNum = 0;
            for (int j = 0; j < fat->files.size(); j++) {
                if (fat->files[j] != nullptr) {
                    Cluster *cluster = fat->files[j]->clusterList;
                    while (cluster != nullptr) {
                        if (cluster->blockIndex == fat->totalBlocks - 1) {
                            fileNum = j;
                            break;
                        }
                        cluster = cluster->next;
                    }
                }
            }
            int nextFileNum = 0;
            for (int j = 0; j < fat->files.size(); j++) {
                if (fat->files[j] != nullptr) {
                    Cluster *cluster = fat->files[j]->clusterList;
                    while (cluster != nullptr) {
                        if (cluster->blockIndex == fat->totalBlocks) {
                            nextFileNum = j;
                            break;
                        }
                        cluster = cluster->next;
                    }
                }
            }
            if (nextFileNum != fileNum) {
                fragmentedBlocks++;

            }

        }}

    return ((float) fragmentedBlocks / (float) fat->totalBlocks) * 100.0f;
}

void FATSYSTEM::defragDisk() {
    cout << "Starting defragmentation..." << endl;

    for (int iteration = 0; iteration < fat->totalBlocks; iteration++) {
        int blockIndex = 0;
        for (int fileIdx = 0; fileIdx < fat->files.size(); fileIdx++) {
            if (fat->files[fileIdx] != nullptr) {
                File *file = fat->files[fileIdx];
                Cluster *cluster = file->clusterList;

                while (cluster != nullptr) {
                    // Find the first free block to move the cluster
                    while (blockIndex < fat->totalBlocks && fat->blockStatus[blockIndex] != BLOCK_FREE) {
                        blockIndex++;
                    }

                    if (blockIndex < fat->totalBlocks) {
                        fat->blockStatus[cluster->blockIndex] = BLOCK_FREE; // Free the old block
                        cluster->blockIndex = blockIndex; // Move to new block
                        fat->blockStatus[blockIndex] = BLOCK_OCCUPIED; // Occupy the new block
                        blockIndex++;
                    }

                    cluster = cluster->next;
                }
            }
        }
    }

    cout << "Defragmentation completed." << endl;
}

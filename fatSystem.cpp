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
    pFat->blockStatus = new BlockStatus;
    pFat->totalBlocks = diskSpace / disk_->getBlockSize();
    pFat->blockSize = disk_->getBlockSize();
    for (int i = 0; i < pFat->totalBlocks; i++) {
        pFat->blockStatus[i] = BLOCK_FREE;
        disk->getBlocks()[i] = " ";
    }

    fat = pFat;
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
    if (strlen(name_) > 31) {
        cout << "Name zu lang" << endl;
        return nullptr;
    }
    qDebug() << "create File 1" << name_;
    vector<string> dataChunks;
    size_t chunk_size = disk->getBlockSize();
    size_t length = data.length();
    int requiredBlocks = 0;
    qDebug() << "create File 2";
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
    newFile->name[31] = '\0';
    newFile->size = length;
    newFile->clusterList = nullptr;
    newFile->isFolder = isFolder;
    newFile->date = QDateTime::currentDateTime();
    qDebug() << "create File 4";
    string parentData = "";
    int result = 0;
    if(parentName != "isRoot"){

        File *fileToRead= findFile(parentName);
        Cluster *cluster = fileToRead->clusterList;

        while (cluster != nullptr) {

            parentData = parentData +disk->getBlocks()[cluster->blockIndex];
            qDebug() << "load parent data" << parentData;
            cluster = cluster->next;

        }
        qDebug() << "create File with parent" << parentData;

        if(parentData.empty()){

            result = editData(parentName, name_);

        }else {

            string dataString = parentData+","+name_;

            result = editData(parentName, dataString);

        }
    }
    qDebug() << "create File 5";
    if(result == 0){ // error handling
        //schauen ob genug platz da


        if (requiredBlocks > getFreeDiskSpace() / fat->blockSize) {
            editData(parentName, parentData);
            qDebug() <<"Nicht genug Platz" ;
            return NULL;
        }
    }


    qDebug() << "create File 6";

    Cluster *lastCluster = nullptr;
    int allocatedBlocks = 0;
    for (int i = 0; i < fat->totalBlocks && allocatedBlocks < requiredBlocks; i++) {
        if (fat->blockStatus[i] == BLOCK_FREE) {
            fat->blockStatus[i] = BLOCK_OCCUPIED;
              qDebug() << "create File 6.1";
            Cluster *cluster = createCluster(i);
              qDebug() << "create File 6.2";
            if (newFile->clusterList == nullptr) {
                newFile->clusterList = cluster;
            } else {
                lastCluster->next = cluster;
                cluster->prev = lastCluster;
            }
            qDebug() << "create File 6.3";
            qDebug() << "Cluster" << cluster->blockIndex;

            lastCluster = cluster;
            allocatedBlocks++;
        }
    }
    fat->files.push_back(newFile);
    showCluster(name_);
    qDebug() << "create File 7" << getNumberOfBlocks("root");


    return NULL;
}

void FATSYSTEM::showCluster(char* fileName){

    qDebug() << "showCluster";

    Cluster *cluster = findFile(fileName)->clusterList;
    qDebug() << "showCluster 3";
    while (cluster != nullptr) {
        qDebug() << "showCluster 3";
        qDebug() << "show Cluster " << cluster->blockIndex;
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
    qDebug() << "data Found" << data;
    return data;
}

QList<File*> FATSYSTEM::getFilesInFolder(char* folderName) {
    QList<File*> filesInFolder;
    qDebug() << "files in " <<folderName;

    string data = getDataOfFile(folderName);

    qDebug() << "files in 1" << data;
    vector<string> names = splitStringIntoParts(data);
    qDebug() << "files in 2" << names.size() ;
    for(int i = 0; i < names.size(); i++){
         qDebug() << "files in " << names[i];
     }
    QList < File * > files;
    for (int i = 0; i < names.size(); i++) {

        filesInFolder.append(findFile(names[i].c_str()));
        qDebug() << "findFile";
    }
     qDebug() << "files in 3" ;
    return filesInFolder;
}

vector<string> FATSYSTEM::splitStringIntoParts(string inputString){
    stringstream ss(inputString);
    qDebug() << "split" << inputString;
    string token;
    vector <string> tokens;


    char delimiter = ',';
    if (inputString != " ") {
        while (getline(ss, token, delimiter)) {
            if ((token != " ")) {
                if(!token.empty()){
                    qDebug() << "splitt" << token;
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
    qDebug() << "return 0 ";
    for(int i = 0; i < fat->files.size(); i++){
        qDebug() << "return 1 ";
        if(fat->files[i] != nullptr){
            qDebug() << "return 2 " << fat->files[i]->name;
            if(strcmp(fileName , fat->files[i]->name) == 0){
            qDebug() << "return 3";
           return fat->files[i];
        }
        }
    }
    return NULL;
}

File* FATSYSTEM::findFile(const char* fileName){
    qDebug() << "return 0 ";
    for(int i = 0; i < fat->files.size(); i++){
        qDebug() << "return 1 ";
        if(fat->files[i] != nullptr){
            qDebug() << "return 2 " << fat->files[i]->name;
            if(strcmp(fileName , fat->files[i]->name) == 0){
                qDebug() << "return 3";
                return fat->files[i];
            }
        }
    }
    return NULL;
}

int FATSYSTEM::editData(char* fileName ,string data){
    qDebug() << "Edit Data 1" << data;
    File* file = findFile(fileName);
    bool isEmpty = false;
    if(data.empty()){
        isEmpty = true;
    }
    qDebug() << "Edit Data 2";
    vector<string> dataChunks;
    size_t chunk_size = fat->blockSize;
    size_t length = data.length();
    int requiredBlocks = 0;



    qDebug() << "Edit Data 3";
    for (size_t i = 0; i < length; i += chunk_size) {
        //  qDebug() << "test for";
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }

    qDebug() << "Edit Data 4";
    //qDebug() << "test 2" << requiredBlocks << "schon " << nodes[fileToBeEditedId]->blockList.size();
    getNumberOfBlocks(fileName);
    qDebug() << "Edit Data 4.11";
    if(getNumberOfBlocks(fileName) < requiredBlocks){

        if (requiredBlocks > getFreeDiskSpace() / fat->blockSize) {
            qDebug() <<"Nicht genug Platz" ;
            return -1;

        }
        //qDebug() << "test 3";
        file->size = length;
        requiredBlocks = requiredBlocks - getNumberOfBlocks(fileName) ;
        qDebug() << "Edit Data 4.1";
        int allocatedBlocks = 0;
        Cluster* lastCluster = getLastClusterOfFile(fileName);
        qDebug() << "Edit Data 4.2";
        for (int i = 0; i < fat->totalBlocks && allocatedBlocks < requiredBlocks; i++) {
            if (fat->blockStatus[i] == BLOCK_FREE) {

                fat->blockStatus[i] = BLOCK_OCCUPIED;

                allocatedBlocks++;

                Cluster *cluster = createCluster(i);
                if (file->clusterList == nullptr) {
                    file->clusterList = cluster;
                } else {
                    lastCluster->next = cluster;
                    cluster->prev = lastCluster;
                }
                lastCluster = cluster;


            }
        }
    }
    qDebug() << "Edit Data 5";
    //qDebug() << "test 4";

    Cluster *cluster = file->clusterList;

    for (int i = 0; cluster != nullptr; i++) {

        if(!isEmpty){
            qDebug() << "full";
            disk->addDataToBlock(cluster->blockIndex, dataChunks[i]);
        } else {
            qDebug() << "empty";
           disk->addDataToBlock(cluster->blockIndex, " ");
        }

        cluster = cluster->next;


    }
    qDebug() << "Edit Data 6";

    //qDebug() << "test 6";
    return 0;
}

int FATSYSTEM::getNumberOfBlocks(char* fileName){
   /* File *file = findFile(fileName);
    qDebug() << "GetNumBlocks 1";
    Cluster *cluster = file->clusterList;
    qDebug() << "GetNumBlocks 2";
    int blocks = 0;
    while (cluster != nullptr) {
        qDebug() << "GetNumBlocks 3" << blocks;
        blocks++;
        cluster = cluster->next;

    }
    qDebug() << "GetNumBlocks 4 " << blocks;
    return blocks;*/
    return 1;
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
    return cluster;
}

QList<File*> FATSYSTEM::getFoldersInFolder( char* folderName) {
    qDebug() << "folFol " << "start " << folderName;


    // qDebug() << "folfol sy s id" << sys->findFile(folderName);
    File *folder = findFile(folderName);
    qDebug() << "folFol " << "folder inode" << folder->name;
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



void FATSYSTEM::deleteFile(const char *fileName) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fat->files[i] != nullptr && strcmp(fat->files[i]->name, fileName) == 0) {
            File *fileToDelete = fat->files[i];
            Cluster *cluster = fileToDelete->clusterList;

            while (cluster != nullptr) {
                fat->blockStatus[cluster->blockIndex] = BLOCK_FREE;
                Cluster *clusterToBeDeleted = cluster;
                cluster = cluster->next;
                free(clusterToBeDeleted);
            }

            free(fileToDelete);
            fat->files[i] = nullptr;
            return;
        }
    }
    cout << "Datei nicht gefunden" << endl;
}

void FATSYSTEM::showFat() {
    qDebug() << "showFat 1";
    for (int i = 0; i < fat->totalBlocks; i++) {
        qDebug() << "showFat 2";
        switch (fat->blockStatus[i]) {
            case BLOCK_RESERVED:
                cout << "R";
                break;
            case BLOCK_DEFECT:
                cout << "D";
                break;
            case BLOCK_FREE:
                cout << "F";
                break;
            case BLOCK_OCCUPIED:

                for (int j = 0; j < MAX_FILES; j++) {
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
                break;
        }
        cout << " ";
    }
    cout << endl;
}

float FATSYSTEM::getFragmentation() {
    /**
        Fragmentierung:
            -Fileindex ändert sich +1
            -Free auf occ / occ auf free +1
            ignorieren von res/def
    Bug:   free blöcke die von defekten oder res ummantelt werden, werden nicht berücksichtigt
 */
    int fragmentedBlocks = 0;

    for (int i = 0; i < fat->totalBlocks - 1; i++) {

        if (fat->blockStatus[i] == BLOCK_OCCUPIED)
            if (fat->blockStatus[i + 1] == BLOCK_FREE) {
                fragmentedBlocks++;
            } else if (fat->blockStatus[i + 1] == BLOCK_OCCUPIED) {
                int fileNum = 0;
                for (int j = 0; j < MAX_FILES; j++) {
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
                int nextFileNum = 0;
                for (int j = 0; j < MAX_FILES; j++) {
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
        for (int j = 0; j < MAX_FILES; j++) {
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
        for (int j = 0; j < MAX_FILES; j++) {
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


    }
    return ((float) fragmentedBlocks / (float) fat->totalBlocks) * 100.0f;
}


void FATSYSTEM::defragDisk() {
    cout << "Starting defragmentation..." << endl;

    int blockIndex = 0; // Reset block index at the beginning

    for (int fileIdx = 0; fileIdx < MAX_FILES; fileIdx++) {
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

    cout << "Defragmentation completed." << endl;
}

// INFO: Legacy code
//
//void defragDisk(Fat* pFat) {
//    cout <<"Defrag" << endl;
//    for (int fileIdx = 0; fileIdx < MAX_FILES; fileIdx++) {
//
//        if (pFat->files[fileIdx] != nullptr) {
//            File* file = pFat->files[fileIdx];
//            Cluster* cluster = file->clusterList;
//            int blockIndex = 0;
//            while (cluster != nullptr) {
//
//                while (blockIndex < pFat->totalBlocks && pFat->blockStatus[blockIndex] != BLOCK_FREE) {
//                    blockIndex++;
//                }
//                pFat->blockStatus[cluster->blockIndex] = BLOCK_FREE;
//                cluster->blockIndex = blockIndex;
//                pFat->blockStatus[blockIndex] = BLOCK_OCCUPIED;
//                cluster = cluster->next;
//            }
//        }
//    }
//    cout <<"Defrag fertig" << endl;
//}




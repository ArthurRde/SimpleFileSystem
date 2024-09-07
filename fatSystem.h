#ifndef FATSYSTEM_H
#define FATSYSTEM_H


#define MAX_FILES 10
#include <vector>
#include "disk.h"
#include <QDateTime>
#include "blockstatus.h"
#include <string.h>
#include <sstream>
#include <QMessageBox>
using namespace std;


struct Cluster {
    int blockIndex;
    struct Cluster *next;
    struct Cluster *prev;
};

struct File {
    char name[32];
    int size;
    char author[32];
    QDateTime date;
    bool isFolder;
    Cluster *clusterList;

};

struct Fat {
    vector<BlockStatus> blockStatus;
    int totalBlocks;
    int blockSize;
    vector<File*> files;
};

class FATSYSTEM {
    Fat * fat;
    Disk* disk;
public:
     FATSYSTEM(int diskSpace, Disk *disk_);
    ~FATSYSTEM();

    Cluster *createCluster(int blockIndex);

    int getFreeDiskSpace();

    File *createFile(char* name_, char* author_, string data,char* parentName, bool isFolder = false);

    void deleteFile(const char *fileName ,bool deleteFolderInFolder = false,bool ignoreFolderTyp  =false);

    void showFat();

    float getFragmentation();

    void defragDisk();
    Disk *getDisk() const;
    void setDisk(Disk *newDisk);

    Fat *getFat() const;
    void setFat(Fat *newFat);
    File* findFile(char* fileName);
    int editData(char *fileName, string data);
    int getNumberOfBlocks(char *fileName);
    Cluster *getLastClusterOfFile(char *fileName);
    void showCluster(char *fileName);
    string getDataOfFile(char *fileName);
    vector<string> splitStringIntoParts(string inputString);
    QList<File*> getFilesInFolder(char *folderName);
    File *findFile(const char *fileName);
    QList<File *> getFoldersInFolder(char *folderName);
    void renameFile(char* folder, char* fileName, char* newName);
    char *findFolderOfFile(char *fileName);
    void removeFileFromFolder(char *fileName, char* folderName);
    int addFileToFolder(char *parentName, char *name_);
    char *getFolderOfFile(char *fileName);
    void deleteFolder(const char *fileName);
    string createUniqueName(char *fileName, char *uniqueText = "-C");
    bool existFile(char *fileName);
    void copyFile(char *fileName, char *folderName);
};

#endif // FATSYSTEM_H

#ifndef INODESYSTEM_H
#define INODESYSTEM_H


#include <vector>
#include <map>
#include <QDate>
#include "disk.h"
#include <string>
#include <string.h>
#include <sstream>
#include <QMessageBox>
#include "blockstatus.h"
using namespace std;

#define BLOCKSIZE 512
#define MAX_FILES 10


struct INode {
    int iNodeNum;
    string name;
    vector<int> blockList;
    string author;
    QDateTime date;
    int size;
    bool isFolder;
};


class INODESYSTEM {
    map<int, INode *> nodes;
    Disk *disk;
    vector <BlockStatus> blockStatus;
    int totalBlocks;

public:

    INODESYSTEM(int diskSize,Disk *disk_);

    void showFat();

    void deleteFile(string fileName,bool deleteFolderInFolder = false,bool ignoreFolderTyp  =false);

    int findFile(string fileName);

    int findFile(int num);

    string createFile(string name_, string author_, string data, string parentName, bool isFolder = false);

    int findLowestNumber();

    int findLowestNumber2();

    int getFreeDiskSpace();

    void renameFile(string fileName, string newName);

    map<int, INode *> getNodes() const;

    void setNodes(const map<int, INode *> &newNodes);

    Disk *getDisk() const;

    void setDisk(Disk *newDisk);

    int editData(int fileToBeEditedId, string data);
    string findFolderofFile(string fileName);
    QList<INode *> getFoldersInFolder(string folderName);
    QList<INode *> getFilesInFolder(string folderName);
    vector<int> splitStringIntoInts(string inputString);
    void deleteFolder(string fileName);

    bool existFileInFolder(string fileName, string folderName);
    string getDataOfFile(string fileName);
    string createUniqueName(string fileName, string uniqueText = "-C");
    void copyFile(string fileName, string folderName);
    bool existFile(string fileName);
};

#endif // INODESYSTEM_H

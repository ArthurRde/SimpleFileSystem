#ifndef INODESYSTEM_H
#define INODESYSTEM_H


#include <vector>
#include <map>
#include <QDate>
#include "disk.h"
#include <string>

using namespace std;

#define BLOCKSIZE 512
#define MAX_FILES 10

enum BlockStatus2 {
    BLOCK_RESERVED2,
    BLOCK_DEFECT2,
    BLOCK_FREE2,
    BLOCK_OCCUPIED2
};

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
    vector <BlockStatus2> blockStatus;
    int totalBlocks;
public:

    INODESYSTEM(int diskSize,Disk *disk_);

    void showFat();

    void deleteFile(string fileName);

    int findFile(string fileName);

    int findFile(int num);

    void createFile(string name_, string author_, string data, string parentName, bool isFolder = false);

    int findLowestNumber();

    int findLowestNumber2();

    int getFreeDiskSpace();

    void renameFile(string fileName, string newName);

    map<int, INode *> getNodes() const;

    void setNodes(const map<int, INode *> &newNodes);

    Disk *getDisk() const;

    void setDisk(Disk *newDisk);

    int editData(int fileToBeEditedId, string data);
};

#endif // INODESYSTEM_H

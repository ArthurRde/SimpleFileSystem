#ifndef FATSYSTEM_H
#define FATSYSTEM_H


#define MAX_FILES 10
#include <vector>
#include "disk.h"
#include <QDateTime>

using namespace std;
enum BlockStatus {
    BLOCK_RESERVED,
    BLOCK_DEFECT,
    BLOCK_FREE,
    BLOCK_OCCUPIED
};

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
    BlockStatus *blockStatus;
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

    void deleteFile(const char *fileName);

    void showFat();

    float getFragmentation();

    void defragDisk();
    Disk *getDisk() const;
    void setDisk(Disk *newDisk);

    Fat *getFat() const;
    void setFat(Fat *newFat);
};

#endif // FATSYSTEM_H

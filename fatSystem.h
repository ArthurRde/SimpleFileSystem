#ifndef FATSYSTEM_H
#define FATSYSTEM_H



#define MAX_FILES 10

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
    char name[12];
    int size;
    Cluster *clusterList;

};

struct Fat {
    BlockStatus *blockStatus;
    int totalBlocks;
    int blockSize;
    File *files[MAX_FILES];

};

class FATSYSTEM
{
public:
    FATSYSTEM();
    Fat *createFat(int diskSize, int blockSize);
    Cluster *createCluster(int blockIndex);
    int getFreeDiskSpace(Fat *pFat);
    File *createFile(Fat *pFat, int szFile, const char *fileName);
    void deleteFile(Fat *pFat, const char *fileName);
    void showFat(Fat *pFat);
    float getFragmentation(Fat *pFat);
    void defragDisk(Fat *pFat);
};

#endif // FATSYSTEM_H

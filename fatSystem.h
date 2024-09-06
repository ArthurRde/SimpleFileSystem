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

class FATSYSTEM {
    Fat * fat;
public:
    FATSYSTEM(int diskSize, int blockSize);
    ~FATSYSTEM();

    Cluster *createCluster(int blockIndex);

    int getFreeDiskSpace();

    File *createFile(int szFile, const char *fileName);

    void deleteFile(const char *fileName);

    void showFat();

    float getFragmentation();

    void defragDisk();
};

#endif // FATSYSTEM_H

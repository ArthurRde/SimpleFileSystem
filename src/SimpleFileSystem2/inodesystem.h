#ifndef INODESYSTEM_H
#define INODESYSTEM_H


#include <iostream>
#include <vector>
#include <list>
#include <map>

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
    char name[12];
    vector<int> blockList;
    char author[12];

};

class INODESYSTEM
{

    map<int, INode*> nodes;


    vector<BlockStatus2> blockStatus;
    int totalBlocks;
public:
    INODESYSTEM(int diskSpace);
    void showFat();
    void deleteFile( const char* fileName);
    int findFile(const char* fileName);
    int findFile(int num);
    void createFile( const char name_[12], const char author_[12], int size_);
    int findLowestNumber();
    int getFreeDiskSpace();

};

#endif // INODESYSTEM_H

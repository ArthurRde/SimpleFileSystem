#include "inodesystem.h"


#include "inodeSystem.h"


using namespace std;

#define BLOCKSIZE 512
#define MAX_FILES 10

INODESYSTEM::INODESYSTEM(int diskSpace){

    totalBlocks = diskSpace/BLOCKSIZE;
    for (int i = 0; i < totalBlocks ; i++)
    {
        blockStatus.push_back(BLOCK_FREE2);
    }
    for (int i = 0; i < MAX_FILES; i++) {
        nodes[i] = NULL;
    }
}

int INODESYSTEM::getFreeDiskSpace() {
    int freeBlocks = 0;
    for (int i = 0; i < totalBlocks; i++) {
        if (blockStatus[i] == BLOCK_FREE2) {
            freeBlocks++;
        }
    }
    return freeBlocks * BLOCKSIZE;
}

int INODESYSTEM::findLowestNumber(){
    int ind = 0;
    for(int i = 0; i< nodes.size(); i ++){
        if(nodes[i] != NULL){
            if(nodes[i]->iNodeNum == ind ){
                ind++;
                i = 0;
            }
        }
    }
    return ind;
}
void INODESYSTEM::createFile( const char name_[12], const char author_[12], int size_){
    int num =findLowestNumber();
    INode *node = new INode();
    strncpy(node->name, name_, 11);
    strncpy(node->author, author_, 11);


    nodes[num] = node;
    nodes[num]->name[11] = '\0';
    int requiredBlocks = size_/BLOCKSIZE;
    cout << requiredBlocks << endl;
    if (requiredBlocks > getFreeDiskSpace() / BLOCKSIZE) {
        cout <<"Nicht genug Platz" << endl;
        return;
    }

    int allocatedBlocks = 0;
    for (int i = 0; i < totalBlocks && allocatedBlocks < requiredBlocks; i++) {
        if (blockStatus[i] == BLOCK_FREE2) {
            cout << "Free" << i << endl;
            blockStatus[i] = BLOCK_OCCUPIED2;
            allocatedBlocks++;
            nodes[num]->blockList.push_back(i);


        }
    }


}

int INODESYSTEM::findFile(int num) {
    if (nodes.find(num) != nodes.end()) {
        return num;
    }
    cout << "cant find File";
    return NULL;
}
int INODESYSTEM::findFile(const char* fileName){
    for (int i = 0; i < MAX_FILES; i++) {
        if (nodes[i] != NULL && strcmp(nodes[i]->name, fileName) == 0) {
            return i;
        }
    }
    cout << "cant find File";
    return -1;
}

void INODESYSTEM::deleteFile( const char* fileName){
    int num = findFile(fileName);
    if(num != -1){
        for(int i = 0; i < nodes[num]->blockList.size(); i++){
            blockStatus[nodes[num]->blockList[i]] = BLOCK_FREE2;
        }
        INode *nodeToDelete =nodes[num];
        nodes.erase(num);
        delete nodeToDelete;
    }
}

void INODESYSTEM::showFat() {
    for (int i = 0; i < totalBlocks; i++) {
        switch (blockStatus[i]) {
        case BLOCK_RESERVED2:
            cout <<"R";
            break;
        case BLOCK_DEFECT2:
            cout <<"D";
            break;
        case BLOCK_FREE2:
            cout <<"F";
            break;
        case BLOCK_OCCUPIED2:
            for (int j = 0; j < MAX_FILES; j++) {
                if (nodes[j] != NULL) {
                    auto at = find(nodes[j]->blockList.begin(), nodes[j]->blockList.end(), i);
                    if (at != nodes[j]->blockList.end()) {
                        cout << j;
                        break;
                    }
                }


            }
            break;
        }

        cout << " ";

    }

    cout <<endl;
}







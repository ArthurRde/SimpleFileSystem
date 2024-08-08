#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#define MAX_FILES 10

using namespace std;

enum BlockStatus {
    BLOCK_RESERVED,
    BLOCK_DEFECT,
    BLOCK_FREE,
    BLOCK_OCCUPIED
};

struct BsCluster {
    int blockIndex;
    struct BsCluster* next;
    struct BsCluster* prev;
};

struct BsFile{
    char name[12];
    int size;
    BsCluster* clusterList;
    
};

struct BsFat{
    BlockStatus* blockStatus;
    int totalBlocks;
    int blockSize;
    BsFile* files[MAX_FILES];
};

BsFat* createBsFat(int diskSize, int blockSize) {
    BsFat* pFat = new BsFat;
    pFat->blockStatus = new BlockStatus;
    pFat->totalBlocks = diskSize / blockSize;
    pFat->blockSize = blockSize;
    for (int i = 0; i < blockSize; i++)
    {
        pFat->blockStatus[i] = BLOCK_FREE;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        pFat->files[i] = NULL;
    }
    return pFat;
}

// Aufgabe 3
BsCluster* createCluster(int blockIndex) {
    BsCluster* cluster = new BsCluster;
    cluster->blockIndex = blockIndex;
    cluster->next = NULL;
    cluster->prev = NULL;
    return cluster;
}

// Aufgabe 5
int getFreeDiskSpace(BsFat* pFat) {
    int freeBlocks = 0;
    for (int i = 0; i < pFat->totalBlocks; i++) {
        if (pFat->blockStatus[i] == BLOCK_FREE) {
            freeBlocks++;
        }
    }
    return freeBlocks * pFat->blockSize;
}

// Aufgabe 6
BsFile* createFile(BsFat* pFat, int szFile, const char* fileName) {
    if (strlen(fileName) > 11) {
        cout << "Name zu lang" << endl;
        return NULL;
    }

    int requiredBlocks = szFile / pFat->blockSize;
    if (requiredBlocks > getFreeDiskSpace(pFat) / pFat->blockSize) {
        cout <<"Nicht genug Platz" << endl;
        return NULL;
    }

    BsFile* newFile = new BsFile;
    strncpy(newFile->name, fileName, 11);
    newFile->name[11] = '\0';
    newFile->size = szFile;
    newFile->clusterList = NULL;

    BsCluster* lastCluster = NULL;
    int allocatedBlocks = 0;
    for (int i = 0; i < pFat->totalBlocks && allocatedBlocks < requiredBlocks; i++) {
        if (pFat->blockStatus[i] == BLOCK_FREE) {
            pFat->blockStatus[i] = BLOCK_OCCUPIED;

            BsCluster* cluster = createCluster(i);
            if (newFile->clusterList == NULL) {
                newFile->clusterList = cluster;
            } else {
                lastCluster->next = cluster;
                cluster->prev = lastCluster;
            }
            lastCluster = cluster;
            allocatedBlocks++;
        }
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (pFat->files[i] == NULL) {
            pFat->files[i] = newFile;
            return newFile;
        }
    }

    cout <<"Zu viele Dateien" << endl;
    free(newFile);
    return NULL;
}

// Aufgabe 7
void deleteFile(BsFat* pFat, const char* fileName) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (pFat->files[i] != NULL && strcmp(pFat->files[i]->name, fileName) == 0) {
            BsFile* fileToDelete = pFat->files[i];
            BsCluster* cluster = fileToDelete->clusterList;

            while (cluster != NULL) {
                pFat->blockStatus[cluster->blockIndex] = BLOCK_FREE;
                BsCluster* clusterToBeDeleted = cluster;
                cluster = cluster->next;
                free(clusterToBeDeleted);
            }

            free(fileToDelete);
            pFat->files[i] = NULL;
            return;
        }
    }
    cout <<"Datei nicht gefunden" << endl;
}

// Aufgabe 8
float getFragmentation(BsFat* pFat) {
    int occupiedBlocks = 0;
    int countConsecutives = 0;
    int freeBlocks = 0;
    int highestConsecutiveFreeBlocks = 0;

    for (int i = 0; i < pFat->totalBlocks; i++) {
        if (pFat->blockStatus[i] != BLOCK_FREE) { //IS OCCUPIED OR OCC,DEFEC AND RESERVED
            occupiedBlocks++;
            countConsecutives = 0;
        }
        if (pFat->blockStatus[i] == BLOCK_FREE) {
            freeBlocks++;
            countConsecutives++;
            if(countConsecutives > highestConsecutiveFreeBlocks){
                highestConsecutiveFreeBlocks = countConsecutives;
            }

        }
    }

    return ((float) freeBlocks - (float) highestConsecutiveFreeBlocks) / (float) occupiedBlocks * 100;
}

// Aufgabe 9
void showFat(BsFat* pFat) {
    for (int i = 0; i < pFat->totalBlocks; i++) {
        switch (pFat->blockStatus[i]) {
            case BLOCK_RESERVED:
                cout <<"R";
                break;
            case BLOCK_DEFECT:
                cout <<"D";
                break;
            case BLOCK_FREE:
                cout <<"F";
                break;
            case BLOCK_OCCUPIED:
                
                for (int j = 0; j < MAX_FILES; j++) {
                    if (pFat->files[j] != NULL) {
                        BsCluster* cluster = pFat->files[j]->clusterList;
                        while (cluster != NULL) {
                            if (cluster->blockIndex == i) {
                                cout<<j;
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
    cout <<endl;
}

// Aufgabe 10

void defragDisk(BsFat* pFat) {
    
    cout <<"Defrag" << endl;

    
    for (int fileIdx = 0; fileIdx < MAX_FILES; fileIdx++) {
        
        if (pFat->files[fileIdx] != NULL) {
           
            BsFile* file = pFat->files[fileIdx];

            
            BsCluster* cluster = file->clusterList;

            
            int blockIndex = 0;

       
            while (cluster != NULL) {
               
                while (blockIndex < pFat->totalBlocks && pFat->blockStatus[blockIndex] != BLOCK_FREE) {
                    blockIndex++;
                }

                
              
                
                    pFat->blockStatus[cluster->blockIndex] = BLOCK_FREE;

                    
                    cluster->blockIndex = blockIndex;

                    
                    pFat->blockStatus[blockIndex] = BLOCK_OCCUPIED;
        

                
                cluster = cluster->next;
            }
        }
    }

  
    cout <<"Defrag fertig" << endl;
}




int main() {
    BsFat* pFat = createBsFat(8 * 1024, 512);

   
    pFat->blockStatus[0] = BLOCK_RESERVED;
    pFat->blockStatus[1] = BLOCK_RESERVED;
    pFat->blockStatus[2] = BLOCK_DEFECT;
    pFat->blockStatus[3] = BLOCK_DEFECT;
    pFat->blockStatus[8] = BLOCK_RESERVED;
    pFat->blockStatus[9] = BLOCK_RESERVED;
    
 
    createFile(pFat,1024,"file1.txt");
    createFile(pFat,512,"BSY-P5.pdf");
    createFile(pFat,2048,"MatheII.txt");

    showFat(pFat);

    deleteFile(pFat, "file1.txt");
    
    showFat(pFat);

    cout << "Frag: "<<getFragmentation(pFat)<<endl;

    defragDisk(pFat);
    showFat(pFat);

    cout << "Frag nach defrag: "<<getFragmentation(pFat)<<endl;

  
    for (int i = 0; i < MAX_FILES; i++) {
        if (pFat->files[i] != NULL) {
            deleteFile(pFat, pFat->files[i]->name);
        }
    }
    free(pFat->blockStatus);
    free(pFat);

    return 0;
}

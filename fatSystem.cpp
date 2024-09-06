#include "fatSystem.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

FATSYSTEM::FATSYSTEM(int diskSize, int blockSize) {
    Fat *pFat = new Fat;
    pFat->blockStatus = new BlockStatus;
    pFat->totalBlocks = diskSize / blockSize;
    pFat->blockSize = blockSize;
    for (int i = 0; i < blockSize; i++) {
        pFat->blockStatus[i] = BLOCK_FREE;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        pFat->files[i] = nullptr;
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

File *FATSYSTEM::createFile(int fileSize, const char *fileName) {
    if (strlen(fileName) > 31) {
        cout << "Name zu lang" << endl;
        return nullptr;
    }
    int requiredBlocks = fileSize / fat->blockSize;
    if (requiredBlocks > getFreeDiskSpace() / fat->blockSize) {
        cout << "Nicht genug Platz" << endl;
        return nullptr;
    }
    File *newFile = new File;
    strncpy(newFile->name, fileName, 31);
    newFile->name[31] = '\0';
    newFile->size = fileSize;
    newFile->clusterList = nullptr;

    Cluster *lastCluster = nullptr;
    int allocatedBlocks = 0;
    for (int i = 0; i < fat->totalBlocks && allocatedBlocks < requiredBlocks; i++) {
        if (fat->blockStatus[i] == BLOCK_FREE) {
            fat->blockStatus[i] = BLOCK_OCCUPIED;

            Cluster *cluster = createCluster(i);
            if (newFile->clusterList == nullptr) {
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
        if (fat->files[i] == nullptr) {
            fat->files[i] = newFile;
            return newFile;
        }
    }
    cout << "Zu viele Dateien" << endl;
    free(newFile);
    return nullptr;
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
    for (int i = 0; i < fat->totalBlocks; i++) {
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




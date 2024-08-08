#include <cstdlib>
#include <cstring>
#include <iostream>

#define MAX_FILES 10

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

Fat *createFat(int diskSize, int blockSize) {
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
    return pFat;
}

Cluster *createCluster(int blockIndex) {
    auto *cluster = new Cluster;
    cluster->blockIndex = blockIndex;
    cluster->next = nullptr;
    cluster->prev = nullptr;
    return cluster;
}

int getFreeDiskSpace(Fat *pFat) {
    int freeBlocks = 0;
    for (int i = 0; i < pFat->totalBlocks; i++) {
        if (pFat->blockStatus[i] == BLOCK_FREE) {
            freeBlocks++;
        }
    }
    return freeBlocks * pFat->blockSize;
}

File *createFile(Fat *pFat, int szFile, const char *fileName) {
    if (strlen(fileName) > 11) {
        cout << "Name zu lang" << endl;
        return nullptr;
    }
    int requiredBlocks = szFile / pFat->blockSize;
    if (requiredBlocks > getFreeDiskSpace(pFat) / pFat->blockSize) {
        cout << "Nicht genug Platz" << endl;
        return nullptr;
    }
    File *newFile = new File;
    strncpy(newFile->name, fileName, 11);
    newFile->name[11] = '\0';
    newFile->size = szFile;
    newFile->clusterList = nullptr;

    Cluster *lastCluster = nullptr;
    int allocatedBlocks = 0;
    for (int i = 0; i < pFat->totalBlocks && allocatedBlocks < requiredBlocks; i++) {
        if (pFat->blockStatus[i] == BLOCK_FREE) {
            pFat->blockStatus[i] = BLOCK_OCCUPIED;

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
        if (pFat->files[i] == nullptr) {
            pFat->files[i] = newFile;
            return newFile;
        }
    }
    cout << "Zu viele Dateien" << endl;
    free(newFile);
    return nullptr;
}

void deleteFile(Fat *pFat, const char *fileName) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (pFat->files[i] != nullptr && strcmp(pFat->files[i]->name, fileName) == 0) {
            File *fileToDelete = pFat->files[i];
            Cluster *cluster = fileToDelete->clusterList;

            while (cluster != nullptr) {
                pFat->blockStatus[cluster->blockIndex] = BLOCK_FREE;
                Cluster *clusterToBeDeleted = cluster;
                cluster = cluster->next;
                free(clusterToBeDeleted);
            }

            free(fileToDelete);
            pFat->files[i] = nullptr;
            return;
        }
    }
    cout << "Datei nicht gefunden" << endl;
}

void showFat(Fat *pFat) {
    for (int i = 0; i < pFat->totalBlocks; i++) {
        switch (pFat->blockStatus[i]) {
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
                    if (pFat->files[j] != nullptr) {
                        Cluster *cluster = pFat->files[j]->clusterList;
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

float getFragmentation(Fat *pFat) {
/**
        Fragmentierung:
            -Fileindex ändert sich +1
            -Free auf occ / occ auf free +1
            ignorieren von res/def
    Bug:   free blöcke die von defekten oder res ummantelt werden, werden nicht berücksichtigt
 */
    int fragmentedBlocks = 0;

    for (int i = 0; i < pFat->totalBlocks - 1; i++) {

        if (pFat->blockStatus[i] == BLOCK_OCCUPIED)
            if (pFat->blockStatus[i + 1] == BLOCK_FREE) {
                fragmentedBlocks++;
            } else if (pFat->blockStatus[i + 1] == BLOCK_OCCUPIED) {
                int fileNum = 0;
                for (int j = 0; j < MAX_FILES; j++) {
                    if (pFat->files[j] != nullptr) {
                        Cluster *cluster = pFat->files[j]->clusterList;
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
                    if (pFat->files[j] != nullptr) {
                        Cluster *cluster = pFat->files[j]->clusterList;
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


        if (pFat->blockStatus[i] == BLOCK_FREE) {
            if (pFat->blockStatus[i + 1] == BLOCK_OCCUPIED) {
                fragmentedBlocks++;
            }
        }
    }


    if ((pFat->blockStatus[pFat->totalBlocks - 2] == BLOCK_OCCUPIED &&
         pFat->blockStatus[pFat->totalBlocks - 1] == BLOCK_FREE) ||
        (pFat->blockStatus[pFat->totalBlocks - 1] == BLOCK_OCCUPIED &&
         pFat->blockStatus[pFat->totalBlocks - 2] == BLOCK_FREE)) {

        fragmentedBlocks++;
    } else {
        int fileNum = 0;
        for (int j = 0; j < MAX_FILES; j++) {
            if (pFat->files[j] != nullptr) {
                Cluster *cluster = pFat->files[j]->clusterList;
                while (cluster != nullptr) {
                    if (cluster->blockIndex == pFat->totalBlocks - 1) {
                        fileNum = j;
                        break;
                    }
                    cluster = cluster->next;
                }
            }
        }
        int nextFileNum = 0;
        for (int j = 0; j < MAX_FILES; j++) {
            if (pFat->files[j] != nullptr) {
                Cluster *cluster = pFat->files[j]->clusterList;
                while (cluster != nullptr) {
                    if (cluster->blockIndex == pFat->totalBlocks) {
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
    return ((float) fragmentedBlocks / (float) pFat->totalBlocks) * 100.0f;
}


void defragDisk(Fat *pFat) {
    cout << "Starting defragmentation..." << endl;

    int blockIndex = 0; // Reset block index at the beginning

    for (int fileIdx = 0; fileIdx < MAX_FILES; fileIdx++) {
        if (pFat->files[fileIdx] != nullptr) {
            File *file = pFat->files[fileIdx];
            Cluster *cluster = file->clusterList;

            while (cluster != nullptr) {
                // Find the first free block to move the cluster
                while (blockIndex < pFat->totalBlocks && pFat->blockStatus[blockIndex] != BLOCK_FREE) {
                    blockIndex++;
                }

                if (blockIndex < pFat->totalBlocks) {
                    pFat->blockStatus[cluster->blockIndex] = BLOCK_FREE; // Free the old block
                    cluster->blockIndex = blockIndex; // Move to new block
                    pFat->blockStatus[blockIndex] = BLOCK_OCCUPIED; // Occupy the new block
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
#include "../fat.hpp"

bool calculateFragmentation(){
    BsFat* pFat = createBsFat(8 * 1024, 512);

    pFat->blockStatus[0] = BLOCK_RESERVED;
    pFat->blockStatus[1] = BLOCK_RESERVED;
    pFat->blockStatus[2] = BLOCK_DEFECT;
    pFat->blockStatus[3] = BLOCK_DEFECT;
    pFat->blockStatus[6] = BLOCK_DEFECT;
    pFat->blockStatus[9] = BLOCK_DEFECT;

    createFile(pFat,1024,"file1.txt");
    createFile(pFat,512,"BSY-P5.pdf");
    createFile(pFat,2048,"MatheII.txt");
    deleteFile(pFat, "file1.txt");
    createFile(pFat, 1024, "hello");
    createFile(pFat, 512, "hello");

    return getFragmentation(pFat) == 18.75;
}

bool calculateFragmentationAllBlocksFree(){
    BsFat* pFat = createBsFat(8 * 1024, 512);

    for (int i = 0; i < pFat->totalBlocks; i++) {
        pFat->blockStatus[i] = BLOCK_FREE;
    }

    return getFragmentation(pFat) == 0;
}
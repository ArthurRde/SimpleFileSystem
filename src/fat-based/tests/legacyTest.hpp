#include "fragmentationTest.hpp"

void legacyTest(){
    Fat* pFat = createFat(8 * 1024, 512);


    pFat->blockStatus[0] = BLOCK_RESERVED;
    pFat->blockStatus[1] = BLOCK_RESERVED;
    pFat->blockStatus[2] = BLOCK_DEFECT;
    pFat->blockStatus[3] = BLOCK_DEFECT;

    createFile(pFat,1024,"file1.txt");
    createFile(pFat,512,"Y-P5.pdf");
    createFile(pFat,2048,"MatheII.txt");

    showFat(pFat);

    deleteFile(pFat, "file1.txt");

    createFile(pFat, 1024, "hello");
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

    free(pFat);
};
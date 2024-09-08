#ifndef CDSYSTEM_H
#define CDSYSTEM_H


#include "cdrom.h"
#include "fatSystem.h"
#include "inodesystem.h"
#include <string>

class CDSYSTEM
{



public:

    vector<CDROM*> ejectedCds;
    vector<CDROM*> insertedCds;
    INODESYSTEM *ISys;
    FATSYSTEM *FSys;

    CDSYSTEM(INODESYSTEM *Isys, FATSYSTEM *Fsys);
    vector<CDROM*> getEjectedCds() const;

    vector<INode*> nodes;
    vector<File*> files;

    void setEjectedCds(const vector<CDROM*> &newEjectedCds);
    void addEjectedCd(string portName_);
    vector<CDROM *> getInsertedCds() const;
    void setInsertedCds(const vector<CDROM *> &newInsertedCds);
    FileEntry *createFileEntry(INode *node);
    vector<Base*> processFiles(vector<INode *> nodes_, vector<File *> files_, CDROM *cd, int leastStartBlock);
    vector<Base *> processFolder(INode *node_, CDROM* cd, int leastStartBlock);
    CDROM* burnCD();
    int getNumberOfFilesInFolder(File *file_);
    int getNumberOfFilesInFolder(INode *node_);
    int getNumberOfFiles(vector<INode *> nodes_, vector<File *> files_);
    int createDataBlocks(CDROM *cd, string data, int leastStartBlock);
    FileEntry *createFileEntry(File *file);
    vector<Base *> processFolder(File *file_, CDROM *cd, int leastStartBlock);
    vector<Base *> processFolder2(INode *node_, CDROM *cd, int leastStartBlock);
    void readFolder(CDROM *cd, int currentSystem, FileEntry *folder, string folderName);
    FileEntry *findEntry(CDROM *cd, int baseBlock);
public slots:
    void readCD(CDROM *cd, int currentSystem);
};

#endif // CDSYSTEM_H

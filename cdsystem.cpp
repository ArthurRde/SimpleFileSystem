#include "cdsystem.h"

vector<CDROM *> CDSYSTEM::getEjectedCds() const {
    return ejectedCds;
}

void CDSYSTEM::setEjectedCds(const vector<CDROM *> &newEjectedCds) {
    ejectedCds = newEjectedCds;
}

void CDSYSTEM::addEjectedCd(string portName_) {
    ejectedCds.push_back(new CDROM(portName_));
}

vector<CDROM *> CDSYSTEM::getInsertedCds() const {
    return insertedCds;
}

void CDSYSTEM::setInsertedCds(const vector<CDROM *> &newInsertedCds) {
    insertedCds = newInsertedCds;
}

vector<Base *> CDSYSTEM::processFiles(vector<INode *> nodes_, vector<File *> files_, CDROM *cd, int leastStartBlock) {
    vector<Base *> baseProccess;

    for (int i = 0; i < nodes_.size(); i++) {
        if (!nodes_[i]->isFolder) {
            FileEntry *en = createFileEntry(nodes_[i]);
            string data = ISys->getDataOfFile(nodes_[i]->name);
            data = nodes_[i]->author + "/" + data;


            en->flags[1] = nodes_[i]->isFolder;
            //en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);
            en->startBlock = createDataBlocks(cd, data, leastStartBlock);

            for (int j = 16; j < cd->baseBlocks.size(); j++) {
                if (cd->baseStatus[i] == BLOCK_FREE) {
                    cd->baseStatus[i] = BLOCK_OCCUPIED;
                    cd->baseBlocks[i] = en;
                    break;
                }
            }
            baseProccess.push_back(en);
        } else {
            vector<Base *> retVal = processFolder(nodes_[i], cd, leastStartBlock);
            FileEntry *en = createFileEntry(nodes_[i]);
            string data = "";
            data = nodes_[i]->author + "/" + data;
            en->flags[1] = nodes_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);




            for (int i = 0; i < retVal.size(); i++) {
                if (retVal[i] != NULL) {
                    data = data + "," + to_string(dynamic_cast<FileEntry *> (retVal[i])->startBlock);
                    for (int j = 16; j < cd->baseBlocks.size(); j++) {
                        if (cd->baseStatus[j] == BLOCK_FREE) {
                            cd->baseStatus[j] = BLOCK_OCCUPIED;
                            cd->baseBlocks[j] = retVal[i];
                            break;
                        }
                    }
                }
            }
            en->startBlock = createDataBlocks(cd, data, leastStartBlock);
            baseProccess.push_back(en);
        }
    }

    for (int i = 0; i < files.size(); i++) {
        if (!files_[i]->isFolder) {
            FileEntry *en = createFileEntry(files_[i]);
            string data = FSys->getDataOfFile(files_[i]->name);
            string str(files_[i]->author);
            data = str + "/" + data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);
            en->startBlock = createDataBlocks(cd, data, leastStartBlock);

            for (int j = 16; j < cd->baseBlocks.size(); j++) {
                if (cd->baseStatus[i] == BLOCK_FREE) {
                    cd->baseStatus[i] = BLOCK_OCCUPIED;
                    cd->baseBlocks[i] = en;
                    break;
                }
            }
            baseProccess.push_back(en);
        } else {
            vector<Base *> retVal = processFolder(files_[i], cd, leastStartBlock);

            FileEntry *en = createFileEntry(files_[i]);
            string data = "";
            string author = files_[i]->author;
            data = author + "/" + data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);

            for (int i = 0; i < retVal.size(); i++) {
                data = data + "," + to_string(dynamic_cast<FileEntry *> (retVal[i])->startBlock);
                for (int j = 16; j < cd->baseBlocks.size(); j++) {
                    if (cd->baseStatus[j] == BLOCK_FREE) {
                        cd->baseStatus[j] = BLOCK_OCCUPIED;
                        cd->baseBlocks[j] = retVal[i];
                        break;
                    }
                }
            }
            en->startBlock = createDataBlocks(cd, data, leastStartBlock);
            baseProccess.push_back(en);
        }
    }

    return baseProccess;


}


vector<Base *> CDSYSTEM::processFolder(INode *node_, CDROM *cd, int leastStartBlock) {
    QList < INode * > nodes2_ = ISys->getFilesInFolder(node_->name);
    int size = nodes2_.size();
    nodes2_.removeAll(nullptr);

    vector<Base *> baseProccess;


    for (int i = 0; i < size; i++) {

        if (!nodes2_[i]->isFolder) {
            FileEntry *en = createFileEntry(nodes2_[i]);
            string data = ISys->getDataOfFile(nodes2_[i]->name);
            data = nodes2_[i]->author + "/" + data;
            en->flags[1] = nodes2_[i]->isFolder;
            en->entryLength = 1;
            en->startBlock = createDataBlocks(cd, data, leastStartBlock);
            baseProccess.push_back(en);
            for (int j = 16; j < cd->baseBlocks.size(); j++) {
                if (cd->baseStatus[i] == BLOCK_FREE) {
                    cd->baseStatus[i] = BLOCK_OCCUPIED;
                    cd->baseBlocks[i] = en;
                    break;
                }
            }
            baseProccess.push_back(en);

        } else {
            vector<Base *> retVal = processFolder(nodes2_[i], cd, leastStartBlock);
            FileEntry *en = createFileEntry(nodes2_[i]);
            string data = "";
            data = nodes2_[i]->author + "/" + data;
            en->flags[1] = nodes2_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);
            for (int i = 0; i < retVal.size(); i++) {

                data = data + "," + to_string(dynamic_cast<FileEntry *> (retVal[i])->startBlock);
                for (int j = 16; j < cd->baseBlocks.size(); j++) {
                    if (cd->baseStatus[j] == BLOCK_FREE) {
                        cd->baseStatus[j] = BLOCK_OCCUPIED;
                        cd->baseBlocks[j] = retVal[i];
                        break;
                    }
                }
            }

            en->startBlock = createDataBlocks(cd, data, leastStartBlock);
            baseProccess.push_back(en);
        }

    }
    return baseProccess;

}

vector<Base *> CDSYSTEM::processFolder(File *file_, CDROM *cd, int leastStartBlock) {
    QList < INode * > files_ = ISys->getFilesInFolder(file_->name);
    vector<Base *> baseProccess;
    for (int i = 0; i < files_.size(); i++) {
        if (!files_[i]->isFolder) {
            FileEntry *en = createFileEntry(files_[i]);
            string data = ISys->getDataOfFile(files_[i]->name);
            data = files_[i]->author + "/" + data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);
            en->startBlock = createDataBlocks(cd, data, leastStartBlock);


            for (int j = 16; j < cd->baseBlocks.size(); j++) {
                if (cd->baseStatus[i] == BLOCK_FREE) {
                    cd->baseStatus[i] = BLOCK_OCCUPIED;
                    cd->baseBlocks[i] = en;
                    break;
                }
            }
            baseProccess.push_back(en);
        } else {
            vector<Base *> retVal = processFolder(files_[i], cd, leastStartBlock);

            FileEntry *en = createFileEntry(files_[i]);
            string data = "";
            data = files_[i]->author + "/" + data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);




            for (int i = 0; i < retVal.size(); i++) {
                data = data + "," + to_string(dynamic_cast<FileEntry *> (retVal[i])->startBlock);
                for (int j = 16; j < cd->baseBlocks.size(); j++) {
                    if (cd->baseStatus[j] == BLOCK_FREE) {
                        cd->baseStatus[j] = BLOCK_OCCUPIED;
                        cd->baseBlocks[j] = retVal[i];
                        break;
                    }
                }
            }

            en->startBlock = createDataBlocks(cd, data, leastStartBlock);
            baseProccess.push_back(en);
        }
    }
    return baseProccess;
}


FileEntry *CDSYSTEM::createFileEntry(INode *node) {
    //TODO: Bei Daten autor in die Daten schreiben

    FileEntry *entry = new FileEntry(node->name, node->size);
    return entry;
}

FileEntry *CDSYSTEM::createFileEntry(File *file) {
    //TODO: Bei Daten autor in die Daten schreiben

    FileEntry *entry = new FileEntry(file->name, file->size);

    return entry;
}

int CDSYSTEM::getNumberOfFiles(vector<INode *> nodes_, vector<File *> files_) {
    int num = 0;
    for (int i = 0; i < nodes_.size(); i++) {
        if (nodes_[i] != NULL) {
            num++;
            if (nodes_[i]->isFolder) {
                num = num + getNumberOfFilesInFolder(nodes_[i]);
            }
        }
    }
    return num;
}

int CDSYSTEM::getNumberOfFilesInFolder(INode *node_) {
    QList < INode * > nodeFolder = ISys->getFilesInFolder(node_->name);
    int num = 0;
    for (int i = 0; i < nodeFolder.size(); i++) {
        if (nodeFolder[i] != NULL) {
            if (!nodeFolder[i]->isFolder) {
                num++;
            } else {
                num++;
                num = num + getNumberOfFilesInFolder(nodeFolder[i]);
            }
        }
    }

    return num;
}

int CDSYSTEM::getNumberOfFilesInFolder(File *file_) {
    QList < File * > filesFolder = FSys->getFilesInFolder(file_->name);
    int num = 0;
    for (int i = 0; i < filesFolder.size(); i++) {
        if (filesFolder[i] != NULL) {
            if (!filesFolder[i]->isFolder) {
                num++;
            } else {
                num++;
                num = num + getNumberOfFilesInFolder(filesFolder[i]);
            }
        }
    }
    return num;
}

int CDSYSTEM::createDataBlocks(CDROM *cd, string data, int leastStartBlock) {
    vector<string> dataChunks;
    size_t chunk_size = cd->blockSize;
    size_t length = data.length();
    int requiredBlocks = 0;


    for (size_t i = 0; i < length; i += chunk_size) {
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }
    int allocatedBlocks = 0;
    int startBlock = -1;
    int pos = 0;
    DataBlock *lastBlock = NULL;
    for (int i = leastStartBlock; i < cd->baseStatus.size() && allocatedBlocks < requiredBlocks; i++) {
        if (cd->baseStatus[i] == BLOCK_FREE) {
            cd->baseStatus[i] = BLOCK_OCCUPIED;
            //cd->baseBlocks[i] = new DataBlock(dataChunks[pos]);
            cd->baseBlocks[i] = new DataBlock(dataChunks[pos]);
            pos++;
            allocatedBlocks++;
            if (startBlock == -1) {
                startBlock = i;
            }
            if (lastBlock != NULL) {
                lastBlock->next = dynamic_cast<DataBlock *> (cd->baseBlocks[i]);
            }
            lastBlock = dynamic_cast<DataBlock *> (cd->baseBlocks[i]);
        }
    }
    return startBlock;

}


CDROM *CDSYSTEM::burnCD() {
    CDROM *cd = NULL;
    for (int i = 0; i < insertedCds.size(); i++) {
        if (!insertedCds[i]->getIsBurned()) {
            cd = insertedCds[i];
            break;
        }
    }
    if (cd == NULL) {
        return NULL;
    }

    for (int i = 0; i < (cd->diskSize / cd->blockSize); i++) {
        cd->baseBlocks.push_back(new DataBlock(""));
    }

    cd->baseStatus = vector<BlockStatus>((cd->diskSize / cd->blockSize), BLOCK_FREE);

    for (int i = 0; i < 16; i++) {
        DataBlock *block = new DataBlock(" ");
        cd->baseBlocks[i] = block;
        cd->baseStatus[i] = BLOCK_RESERVED;
    }

    bool findFree = true;
    int pvD = -1;
    for (int i = 16; findFree; i++) {
        if (cd->baseStatus[i] == BLOCK_FREE) {
            cd->baseStatus[i] = BLOCK_OCCUPIED;


            pvD = i;
            findFree = false;
        }
    }


    INode *root = new INode();
    root->name = "root";
    root->isFolder = true;
    root->author = "system";

    string rootData = "";
    FileEntry *en = createFileEntry(root);

    //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);



    int numOfFileEntrys = getNumberOfFiles(nodes, files) + pvD;
    vector<Base *> process = processFiles(nodes, files, cd, numOfFileEntrys);
    int pos = 0;

    for (int i = 0; i < process.size(); i++) {
        FileEntry *en1 = dynamic_cast<FileEntry *>(process[i]);
        for (int i = pvD; i < cd->baseBlocks.size(); i++) {
            if (cd->baseStatus[i] == BLOCK_FREE) {
                cd->baseStatus[i] = BLOCK_OCCUPIED;
                cd->baseBlocks[i] = en1;
                break;
            }
        }
        rootData = rootData + "," + to_string(en1->startBlock);
    }
    root->size = rootData.length();
    int rootStart = createDataBlocks(cd, rootData, numOfFileEntrys);
    en->startBlock = rootStart;

    for (int i = 0; i < cd->baseBlocks.size(); i++) {
    }
    for (int i = pvD; i < cd->baseBlocks.size(); i++) {
        if (cd->baseStatus[i] == BLOCK_FREE) {
            cd->baseStatus[i] = BLOCK_OCCUPIED;
            cd->baseBlocks[i] = en;
            break;
        }
    }

    PrimaryVolumeDescriptor *vd = new PrimaryVolumeDescriptor(en);
    vd->rootDirectoryEntry = en;
    cd->baseBlocks[pvD] = vd;

    cd->setIsBurned(true);

    nodes.clear();
    files.clear();
    return cd;

}

vector<string> splitAuthor(string inputString) {
    stringstream ss(inputString);
    string token;
    vector<string> tokens;
    char delimiter = '/';
    if (inputString != " ") {
        while (getline(ss, token, delimiter)) {
            if ((token != " ")) {
                if (!token.empty()) {
                    tokens.push_back(token);;
                }
            }
        }
    }
    return tokens;
}

vector<int> splitBlocks(string inputString) {
    stringstream ss(inputString);
    string token;
    vector<string> tokens;
    vector<int> nums;
    char delimiter = ',';
    if (inputString != " ") {
        while (getline(ss, token, delimiter)) {
            if ((token != " ")) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    nums.push_back(stoi(token));
                }
            }
        }
    }
    return nums;
}

void CDSYSTEM::readCD(CDROM *cd, int currentSystem) {
    if (cd->getIsBurned()) {
        bool foundPV = false;

        for (int i = 0; !foundPV; i++) {
            if (dynamic_cast<PrimaryVolumeDescriptor *>(cd->baseBlocks[i])) {
                FileEntry *root = dynamic_cast<PrimaryVolumeDescriptor *>(cd->baseBlocks[i])->rootDirectoryEntry;
                string data = "";
                bool foundNext = true;
                int startBlock = root->startBlock;
                DataBlock *dataB = dynamic_cast<DataBlock *>(cd->baseBlocks[startBlock]);
                for (int i = 0; foundNext; i++) {
                    qDebug() << dataB->data;
                    data = data + dataB->data;
                    if (dataB->next != NULL) {
                        dataB = dataB->next;
                    } else {
                        foundNext = false;
                    }
                }


                vector<int> nums = splitBlocks(data);
                for (int i = 0; i < nums.size(); i++) {

                    FileEntry *entr = findEntry(cd, nums[i]);

                    if (entr != nullptr) {


                        string dataEntr;
                        bool foundNext = true;
                        int startBlock2 = entr->startBlock;
                        DataBlock *dataB2 = dynamic_cast<DataBlock *>(cd->baseBlocks[startBlock2]);

                        for (int i = 0; foundNext; i++) {
                            dataEntr = dataEntr + dataB2->data;
                            if (dataB2->next != NULL) {
                                dataB2 = dataB2->next;
                            } else {
                                foundNext = false;
                            }
                        }

                        vector<string> str = splitAuthor(dataEntr);
                        bool authorM = false;
                        if (str[1].empty()) {
                            str[1] = str[0];
                            authorM = true;
                        }
                        if (entr->fileName != "root") {
                            if (currentSystem == 1) {
                                if (entr->flags[1] == false) {
                                    if (authorM) {
                                        ISys->createFile(entr->fileName, "system", str[0], "root");
                                    } else {
                                        ISys->createFile(entr->fileName, str[1], str[1], "root");
                                    }
                                } else {
                                    string folderName = ISys->createFile(entr->fileName, str[0], " ", "root", true);
                                    readFolder(cd, currentSystem, entr, folderName);


                                }

                            } else {
                                if (entr->flags[1] == false) {
                                    if (authorM) {
                                        string name = FSys->createUniqueName(const_cast<char *>(entr->fileName.c_str()),
                                                                             "-D");
                                        FSys->createFile(const_cast<char *>(name.c_str()), "system", str[0], "root");
                                    } else {
                                        string name = FSys->createUniqueName(const_cast<char *>(entr->fileName.c_str()),
                                                                             "-D");
                                        FSys->createFile(const_cast<char *>(name.c_str()),
                                                         const_cast<char *>(str[1].c_str()), str[1], "root");
                                    }
                                } else {
                                    string name = FSys->createUniqueName(const_cast<char *>(entr->fileName.c_str()),
                                                                         "-D");
                                    char *folderName = FSys->createFile(const_cast<char *>(name.c_str()),
                                                                        const_cast<char *>(str[0].c_str()), " ", "root",
                                                                        true)->name;
                                    readFolder(cd, currentSystem, entr, folderName);
                                }

                            }
                        }

                    }

                }
                foundPV = true;
            }

        }
    }
}


void CDSYSTEM::readFolder(CDROM *cd, int currentSystem, FileEntry *folder, string folderName) {

    string data = "";
    bool foundNext = true;
    int startBlock = folder->startBlock;
    DataBlock *dataB = dynamic_cast<DataBlock *>(cd->baseBlocks[startBlock]);

    for (int i = 0; foundNext; i++) {
        data = data + dataB->data;
        if (dataB->next != NULL) {
            dataB = dataB->next;
        } else {
            foundNext = false;
        }
    }

    bool authorM = false;
    vector<string> str = splitAuthor(data);
    if (str[1].empty()) {
        str[1] = str[0];
        authorM = true;
    }
    vector<int> nums = splitBlocks(str[1]);
    for (int i = 0; i < nums.size(); i++) {
        FileEntry *entr = findEntry(cd, nums[i]);

        if (entr != nullptr) {


            string dataEntr;
            bool foundNext = true;
            int startBlock2 = entr->startBlock;
            DataBlock *dataB2 = dynamic_cast<DataBlock *>(cd->baseBlocks[startBlock2]);
            for (int i = 0; foundNext; i++) {
                dataEntr = dataEntr + dataB2->data;
                if (dataB2->next != NULL) {
                    dataB2 = dataB2->next;
                } else {
                    foundNext = false;
                }
            }

            vector<string> str = splitAuthor(dataEntr);
            if (entr->fileName != "root") {
                if (currentSystem == 1) {
                    if (entr->flags[1] == false) {
                        if (authorM) {
                            ISys->createFile(entr->fileName, "system", str[0], folderName);
                        } else {
                            ISys->createFile(entr->fileName, str[1], str[1], folderName);
                        }
                    } else {
                        string folderName2 = ISys->createFile(entr->fileName, str[0], " ", folderName, true);
                        readFolder(cd, currentSystem, entr, folderName2);


                    }

                } else {
                    if (entr->flags[1] == false) {
                        if (authorM) {
                            string name = FSys->createUniqueName(const_cast<char *>(entr->fileName.c_str()), "-D");
                            FSys->createFile(const_cast<char *>(name.c_str()), "system", str[0],
                                             const_cast<char *>(folderName.c_str()));
                        } else {
                            string name = FSys->createUniqueName(const_cast<char *>(entr->fileName.c_str()), "-D");
                            FSys->createFile(const_cast<char *>(name.c_str()), const_cast<char *>(str[1].c_str()),
                                             const_cast<char *>(str[1].c_str()),
                                             const_cast<char *>(folderName.c_str()));
                        }
                    } else {
                        string name = FSys->createUniqueName(const_cast<char *>(entr->fileName.c_str()), "-D");
                        string folderName2 = FSys->createFile(const_cast<char *>(name.c_str()),
                                                              const_cast<char *>(str[1].c_str()), " ",
                                                              const_cast<char *>(folderName.c_str()), true)->name;
                        readFolder(cd, currentSystem, entr, folderName2);


                    }
                }
            }


        }

    }
}


FileEntry *CDSYSTEM::findEntry(CDROM *cd, int baseBlock) {
    for (int i = 0; i < cd->baseBlocks.size(); i++) {

        FileEntry *entr = nullptr;
        entr = dynamic_cast<FileEntry *>(cd->baseBlocks[i]);

        if (entr != nullptr) {
            if (entr->startBlock == baseBlock) {
            return entr;
            }
        }
    }
    return nullptr;
}

CDSYSTEM::CDSYSTEM(INODESYSTEM *Isys, FATSYSTEM *Fsys) : ISys(Isys), FSys(Fsys) {}

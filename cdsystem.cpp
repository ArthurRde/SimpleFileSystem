#include "cdsystem.h"

vector<CDROM*> CDSYSTEM::getEjectedCds() const
{
    qDebug() << "return ejec CD";
    return ejectedCds;
}

void CDSYSTEM::setEjectedCds(const vector<CDROM*> &newEjectedCds)
{
    ejectedCds = newEjectedCds;
}

void CDSYSTEM::addEjectedCd(string portName_){
    qDebug() << "add ejec CD";
    ejectedCds.push_back(new CDROM(portName_));
}
vector<CDROM *> CDSYSTEM::getInsertedCds() const
{
    return insertedCds;
}

void CDSYSTEM::setInsertedCds(const vector<CDROM *> &newInsertedCds)
{
    insertedCds = newInsertedCds;
}

vector<Base*> CDSYSTEM::processFiles(vector<INode*> nodes_, vector<File*> files_, CDROM* cd, int leastStartBlock){
    vector<Base*> baseProccess;

    for(int i = 0; i < nodes_.size(); i++){
        qDebug() << "process files 2";
        if(!nodes_[i]->isFolder){
        FileEntry* en = createFileEntry(nodes_[i]);
        string data = ISys->getDataOfFile(nodes_[i]->name);
        data = nodes_[i]->author+"/"+data;


         en->flags[1] = nodes_[i]->isFolder;
        //en->entryLength = 1;
        //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);
        en->startBlock = createDataBlocks(cd,data,leastStartBlock);

        qDebug() << "process files 3";
        for(int j = 16; j < cd->baseBlocks.size(); j++){
            if(cd->baseStatus[i] == BLOCK_FREE){
                cd->baseStatus[i] = BLOCK_OCCUPIED;
                cd->baseBlocks[i] = en;
                qDebug() << "proccessed file added " << dynamic_cast<FileEntry*>(cd->baseBlocks[i])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[i])->startBlock << i;
                break;
            }
        }
        baseProccess.push_back(en);
        } else {
            qDebug() << "process files folder 1";
            qDebug() << "is folder";
            vector<Base*> retVal = processFolder(nodes_[i],cd, leastStartBlock);
            qDebug() << "process folder 6" ;
            FileEntry* en = createFileEntry(nodes_[i]);
            string data = "";
            data = nodes_[i]->author+"/"+data;
            en->flags[1] = nodes_[i]->isFolder;
            en->entryLength = 1;
            qDebug() << "process folder 6" << data << retVal.size();
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);




            for(int i = 0; i < retVal.size(); i++){
                if(retVal[i] != NULL){
                data = data + "," + to_string(dynamic_cast<FileEntry*> (retVal[i])->startBlock);
                for(int j = 16; j < cd->baseBlocks.size(); j++){
                    if(cd->baseStatus[j] == BLOCK_FREE){
                        cd->baseStatus[j] = BLOCK_OCCUPIED;
                        cd->baseBlocks[j] = retVal[i];
                        qDebug() << "proccessed file added 1" << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->startBlock << i;
                        break;
                    }
                }
                }
            }
            qDebug() << "process folder 7";
            en->startBlock = createDataBlocks(cd,data,leastStartBlock);
            qDebug() << " #########################################" << en->fileName << en->startBlock;
            baseProccess.push_back(en);
        }
    }

    for(int i = 0; i < files.size(); i++){
        if(!files_[i]->isFolder){
            FileEntry* en = createFileEntry(files_[i]);
            string data =  FSys->getDataOfFile(files_[i]->name);
            string str(files_[i]->author);
            data = str+"/"+data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);
            en->startBlock = createDataBlocks(cd,data,leastStartBlock);

            for(int j = 16; j < cd->baseBlocks.size(); j++){
                if(cd->baseStatus[i] == BLOCK_FREE){
                    cd->baseStatus[i] = BLOCK_OCCUPIED;
                    cd->baseBlocks[i] = en;
                    qDebug() << "proccessed file added 2" << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->startBlock << i;
                    break;
                }
            }
            baseProccess.push_back(en);
        } else {
            vector<Base*> retVal = processFolder(files_[i], cd, leastStartBlock);

            FileEntry* en = createFileEntry(files_[i]);
            string data = "";
            string author = files_[i]->author;
            qDebug() << "######## author" << author;
            data = author+"/"+data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);




            for(int i = 0; i < retVal.size(); i++){
                data = data + "," + to_string(dynamic_cast<FileEntry*> (retVal[i])->startBlock);
                for(int j = 16; j < cd->baseBlocks.size(); j++){
                    if(cd->baseStatus[j] == BLOCK_FREE){
                        cd->baseStatus[j] = BLOCK_OCCUPIED;
                        cd->baseBlocks[j] = retVal[i];
                        qDebug() << "proccessed file added 3" << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->startBlock << i;
                        break;
                    }
                }
            }
            en->startBlock = createDataBlocks(cd,data,leastStartBlock);
            qDebug() << " #########################################" << en->fileName << en->startBlock << data;
            baseProccess.push_back(en);
        }
    }

    return baseProccess;


}



vector<Base*> CDSYSTEM::processFolder(INode* node_, CDROM* cd, int leastStartBlock){
    QList<INode*> nodes2_ = ISys->getFilesInFolder(node_->name);
    int size = nodes2_.size();
    qDebug() << "process folder 1.0" << nodes2_.size();
    nodes2_.removeAll(nullptr);
    qDebug() << "process folder 1.1 " << nodes2_.size();
    for(int i = 0; i<nodes2_.size();  i++){
        qDebug() << nodes2_[i]->isFolder;
    }

    vector<Base*> baseProccess;


    for(int i = 0; i<size;  i++){

        if(!nodes2_[i]->isFolder){
            qDebug() << "is not folder";
            FileEntry* en = createFileEntry(nodes2_[i]);
            string data = ISys->getDataOfFile(nodes2_[i]->name);
             data = nodes2_[i]->author+"/"+data;
            en->flags[1] = nodes2_[i]->isFolder;
            en->entryLength = 1;
            en->startBlock = createDataBlocks(cd,data,leastStartBlock);
            baseProccess.push_back(en);
            for(int j = 16; j < cd->baseBlocks.size(); j++){
                if(cd->baseStatus[i] == BLOCK_FREE){
                    cd->baseStatus[i] = BLOCK_OCCUPIED;
                    cd->baseBlocks[i] = en;
                    qDebug() << "proccessed file added 5" << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->startBlock << i;
                    break;
                }
            }
            baseProccess.push_back(en);

        } else{
            qDebug() << "is folder";
            vector<Base*> retVal = processFolder(nodes2_[i],cd, leastStartBlock);
            qDebug() << "process folder 6" << nodes2_[i]->name;
            FileEntry* en = createFileEntry(nodes2_[i]);
            string data = "";
            data = nodes2_[i]->author+"/"+data;
            en->flags[1] = nodes2_[i]->isFolder;
            en->entryLength = 1;
            qDebug() << "process folder 6" << data;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);






                for(int i = 0; i < retVal.size(); i++){

                    data = data + "," + to_string(dynamic_cast<FileEntry*> (retVal[i])->startBlock);
                    qDebug() << "hello 1" << data;
                    for(int j = 16; j < cd->baseBlocks.size(); j++){
                        qDebug() << "hello 2" << cd->baseStatus[j];
                        if(cd->baseStatus[j] == BLOCK_FREE){
                            cd->baseStatus[j] = BLOCK_OCCUPIED;
                            qDebug() << "hello 3";
                            cd->baseBlocks[j] = retVal[i];
                            qDebug() << "hello 4";
                            qDebug() << "proccessed file added 6" << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->startBlock << i;
                            break;
                        }
                    }
                }


            qDebug() << "process folder 7";
            en->startBlock = createDataBlocks(cd,data,leastStartBlock);
            baseProccess.push_back(en);

            qDebug() << "is folder" << data;
        }

    }

    qDebug() << "end" << baseProccess.size();
    return baseProccess;

}

vector<Base*> CDSYSTEM::processFolder(File* file_, CDROM* cd, int leastStartBlock){
    QList<INode*> files_ = ISys->getFilesInFolder(file_->name);
    vector<Base*> baseProccess;
    for(int i = 0; i<files_.size();  i++){
        if(!files_[i]->isFolder){
            FileEntry* en = createFileEntry(files_[i]);
            string data = ISys->getDataOfFile(files_[i]->name);
            data = files_[i]->author+"/"+data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);
            en->startBlock = createDataBlocks(cd,data,leastStartBlock);


                for(int j = 16; j < cd->baseBlocks.size(); j++){
                    if(cd->baseStatus[i] == BLOCK_FREE){
                        cd->baseStatus[i] = BLOCK_OCCUPIED;
                        cd->baseBlocks[i] = en;
                        qDebug() << "proccessed file added 7" << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->startBlock << i;
                        break;
                    }
                }
            baseProccess.push_back(en);
        } else {
            vector<Base*> retVal = processFolder(files_[i],cd, leastStartBlock);

            FileEntry* en = createFileEntry(files_[i]);
            string data = "";
            data = files_[i]->author+"/"+data;
            en->flags[1] = files_[i]->isFolder;
            en->entryLength = 1;
            //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);




                for(int i = 0; i < retVal.size(); i++){
                    data = data + "," + to_string(dynamic_cast<FileEntry*> (retVal[i])->startBlock);
                    for(int j = 16; j < cd->baseBlocks.size(); j++){
                        if(cd->baseStatus[j] == BLOCK_FREE){
                            cd->baseStatus[j] = BLOCK_OCCUPIED;
                            cd->baseBlocks[j] = retVal[i];
                            qDebug() << "proccessed file added 8" << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->fileName << "at startblock " << dynamic_cast<FileEntry*>(cd->baseBlocks[j])->startBlock << i;
                                break;
                        }
                    }
                }

            en->startBlock = createDataBlocks(cd,data,leastStartBlock);
            baseProccess.push_back(en);
        }
    }
    return baseProccess;
}




FileEntry* CDSYSTEM::createFileEntry(INode* node){
    //bei daten noch autor in die Daten Schreiben

    FileEntry* entry = new FileEntry(node->name,node->size);
    qDebug() << "entry created -----------------------" << entry->fileName;
    return entry;
}

FileEntry* CDSYSTEM::createFileEntry(File* file){
    //bei daten noch autor in die Daten Schreiben

    FileEntry* entry = new FileEntry(file->name,file->size);

    return entry;
}

int CDSYSTEM::getNumberOfFiles(vector<INode*> nodes_, vector<File*> files_){
    int num = 0;
    for(int i = 0; i < nodes_.size(); i++){
        if(nodes_[i]!= NULL){
            num++;
            if(nodes_[i]->isFolder){
                num = num + getNumberOfFilesInFolder(nodes_[i]);
            }
        }
    }
    return num;
}

int CDSYSTEM::getNumberOfFilesInFolder(INode* node_){
    QList<INode*> nodeFolder = ISys->getFilesInFolder(node_->name);
    int num = 0;
    for(int i = 0; i < nodeFolder.size(); i++){
        if(nodeFolder[i]!= NULL){
        if(!nodeFolder[i]->isFolder){
                num++;
        }else {
            num++;
            num = num + getNumberOfFilesInFolder(nodeFolder[i]);
        }
        }
    }

    return num;
}

int CDSYSTEM::getNumberOfFilesInFolder(File* file_){
    QList<File*> filesFolder = FSys->getFilesInFolder(file_->name);
    int num = 0;
    for(int i = 0; i < filesFolder.size(); i++){
        if(filesFolder[i]!= NULL){
            if(!filesFolder[i]->isFolder){
                num++;
            }else {
                num++;
                num = num + getNumberOfFilesInFolder(filesFolder[i]);
            }
        }
    }
    return num;
}

int CDSYSTEM::createDataBlocks(CDROM *cd,string data, int leastStartBlock){
    qDebug() << "create data 1";
    vector<string> dataChunks;
    size_t chunk_size = cd->blockSize;
    size_t length = data.length();
    int requiredBlocks = 0;


    qDebug() << "create data 2";
    for (size_t i = 0; i < length; i += chunk_size) {
        //  qDebug() << "test for";
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }
    qDebug() << "create data 3" << data;
     int allocatedBlocks = 0;
    int startBlock = -1;
     int pos = 0;
    DataBlock *lastBlock = NULL;
     qDebug() << "create data 4";
     for(int i = leastStartBlock; i < cd->baseStatus.size() && allocatedBlocks < requiredBlocks; i++){
         qDebug() << "create data 4.1";
        if(cd->baseStatus[i] == BLOCK_FREE){
             qDebug() << "create data 4.2";
            cd->baseStatus[i] = BLOCK_OCCUPIED;
             qDebug() << "create data 4.3" << dataChunks.size() << dataChunks[pos];
            //cd->baseBlocks[i] = new DataBlock(dataChunks[pos]);
             cd->baseBlocks[i] = new DataBlock(dataChunks[pos]);
             qDebug() << "+++++++++++++++++" << dynamic_cast<DataBlock*>(cd->baseBlocks[i])->data << i;
                 pos++;
            allocatedBlocks++;
                 qDebug() << "create data 5" << startBlock;
            if(startBlock == -1){
                startBlock = i;
            }
             qDebug() << "create data 5.2" << startBlock;
            if(lastBlock != NULL){
                lastBlock->next = dynamic_cast<DataBlock*> (cd->baseBlocks[i]);
            }
            lastBlock = dynamic_cast<DataBlock*> (cd->baseBlocks[i]);
        }
    }
     qDebug() << "create data 6" << startBlock << dynamic_cast<DataBlock*> (cd->baseBlocks[startBlock])->data;
    return startBlock;

}






CDROM* CDSYSTEM::burnCD(){
    CDROM* cd = NULL;
    for(int i = 0; i < insertedCds.size(); i++){
        if(!insertedCds[i]->getIsBurned()){
            cd = insertedCds[i];
            break;
        }
    }
    if(cd == NULL){
        return NULL;
    }

    qDebug() << "cd sys 1";
   /** for(int i = 0; i < insertedCds.size(); i++){
        if(insertedCds[i] != NULL){
            if(!insertedCds[i]->getIsBurned()){
                cd = insertedCds[i];
                break;
            }
        }
    }*/

    qDebug() << "cd sys 2";
    for(int i = 0; i < (cd->diskSize/cd->blockSize); i++){
        cd->baseBlocks.push_back(new DataBlock(""));
    }

    cd->baseStatus = vector<BlockStatus>((cd->diskSize/cd->blockSize), BLOCK_FREE);


    qDebug() << "cd sys 3" << cd->baseBlocks.size();





    for(int i = 0; i< 16; i++){
        DataBlock *block = new DataBlock(" ");
        qDebug() << "cd sys 3.1";
        cd->baseBlocks[i] = block;
        qDebug() << "cd sys 3.2";
        cd->baseStatus[i] = BLOCK_RESERVED;
        qDebug() << "cd sys 3.3";
    }


    qDebug() << "cd sys 4";
    bool findFree = true;
    int pvD = -1;
    for(int i = 16;  findFree; i++){
        qDebug() << "search pvD";
        if(cd->baseStatus[i] == BLOCK_FREE){
            cd->baseStatus[i] = BLOCK_OCCUPIED;



            pvD = i;
            qDebug() << "PVD ++++++++++++++++++++++++++++++++++++++++++++" << pvD;
            findFree = false;
        }
    }




    qDebug() << "cd sys 5";
    INode *root = new INode();
    root->name = "root";
    root->isFolder = true;
    root->author = "system";

    string rootData = "";
    qDebug() << "cd sys 6";
    //add to Folder Data Block Num of start

  /* for(int i = 0; i < nodes.size(); i++){
        rootData = rootData + ","+ nodes[i]->name;
    }
    for(int i = 0; i < files.size(); i++){
        rootData = rootData + ","+files[i]->name;
    }*/



    qDebug() << "cd sys 6.1";
    FileEntry *en = createFileEntry(root);
    qDebug() << "cd sys 6.2";

        //en->padding.resize((en->entryLength % 2 == 0) ? 0 : 1, 0);


    qDebug() << "cd sys 7";

    int numOfFileEntrys = getNumberOfFiles(nodes,files)+pvD;
    qDebug() << "cd sys 8";
    vector<Base*> process = processFiles(nodes,files,cd,numOfFileEntrys);
    int pos = 0;
    qDebug() << "cd sys 9";

    qDebug() << "cd sys 10";
    for(int i = 0; i < process.size(); i++){
        FileEntry *en1 = dynamic_cast<FileEntry*>(process[i]);
        qDebug() << "files 2222222222222222222222222 " << dynamic_cast<FileEntry*>(process[i])->fileName;
        for(int i = pvD; i < cd->baseBlocks.size(); i++){
            qDebug() << "root added find" << i << BLOCK_FREE;
            if(cd->baseStatus[i] == BLOCK_FREE){
                cd->baseStatus[i] = BLOCK_OCCUPIED;
                cd->baseBlocks[i] = en1;
                qDebug() << "root added -----------------------------------" << i << en->startBlock;
                break;
            }
        }
        rootData = rootData + ","+ to_string(en1->startBlock);
    }/**

    qDebug() << "cd sys 11" << rootData;
    for(int i = 0; i < files.size(); i++){
        for(int j = 0; j < cd->baseBlocks.size(); j++){

            if(dynamic_cast<FileEntry*>(cd->baseBlocks[j])){
                FileEntry* en1 = dynamic_cast<FileEntry*>(cd->baseBlocks[j]);
                qDebug() << en1->fileName;
                if(strcmp(en1->fileName.c_str() , files[i]->name) == 0){
                    rootData = rootData + ","+ to_string(en->startBlock);
                }
            }
        }
    }*/
    qDebug() << "cd sys 12" << rootData;
    root->size = rootData.length();
    int rootStart = createDataBlocks(cd, rootData,numOfFileEntrys);
    en->startBlock = rootStart;
    qDebug() << "root added " << en->startBlock;

    for(int i = 0; i < cd->baseBlocks.size(); i++){
        qDebug()<<cd->baseStatus[i];
    }
    for(int i = pvD; i < cd->baseBlocks.size(); i++){
        qDebug() << "root added find" << i << BLOCK_FREE;
        if(cd->baseStatus[i] == BLOCK_FREE){
            cd->baseStatus[i] = BLOCK_OCCUPIED;
            cd->baseBlocks[i] = en;
            qDebug() << "root added -----------------------------------" << i << en->startBlock;
            break;
        }
    }

    qDebug() << "cd sys 13";
    PrimaryVolumeDescriptor *vd = new PrimaryVolumeDescriptor(en);
    vd->rootDirectoryEntry =en;
    cd->baseBlocks[pvD] = vd;


    qDebug() << "cd sys 14";


    cd->setIsBurned(true);

    nodes.clear();
    files.clear();
    return cd;

}

vector<string> splitAuthor(string inputString){
    stringstream ss(inputString);
    //qDebug() << "split" << inputString;
    string token;
    vector <string> tokens;


    char delimiter = '/';
    if (inputString != " ") {
        while (getline(ss, token, delimiter)) {
            if ((token != " ")) {
                if(!token.empty()){
                    //              qDebug() << "splitt" << token;
                    tokens.push_back(token);
                    ;
                    //qDebug() << "splitStringIntoInts: token -> " << token;

                }
            }
        }
    }
    return tokens;
}

vector<int> splitBlocks(string inputString){
    stringstream ss(inputString);
    string token;
    vector <string> tokens;
    vector<int> nums;
    char delimiter = ',';
    if (inputString != " ") {
        while (getline(ss, token, delimiter)) {
            if ((token != " ")) {
                if(!token.empty()){
                    tokens.push_back(token);
                    //qDebug() << "splitStringIntoInts: token -> " << token;
                    nums.push_back(stoi(token));
                }
            }
        }
    }
    return nums;
}

void CDSYSTEM::readCD( CDROM* cd , int currentSystem){
    if(cd->getIsBurned()){
        bool foundPV = false;

        for(int i = 0; !foundPV; i++){
        if(dynamic_cast<PrimaryVolumeDescriptor*>(cd->baseBlocks[i])){
            FileEntry* root = dynamic_cast<PrimaryVolumeDescriptor*>(cd->baseBlocks[i])->rootDirectoryEntry;
            qDebug() << "root";
            string data = "";
            bool foundNext = true;
            int startBlock = root->startBlock;
            DataBlock *dataB = dynamic_cast<DataBlock*>(cd->baseBlocks[startBlock]);

            qDebug() << "root2" << startBlock;
            for(int i = 0; foundNext; i++){
                qDebug() << dataB->data;
                data = data + dataB->data;
                if(dataB->next != NULL){
                dataB = dataB->next;
                } else {
                    foundNext = false;
                }
            }
            qDebug() << "root3" << data;



             vector<int> nums = splitBlocks(data);
            qDebug() << "start block" << nums[0];
             for(int i = 0; i < nums.size(); i++){
                 qDebug() << "start block" << i;

                 FileEntry *entr = findEntry(cd, nums[i]);

                     if(entr != nullptr){




                         string dataEntr;
                         bool foundNext = true;
                         int startBlock2 = entr->startBlock;
                         DataBlock *dataB2 = dynamic_cast<DataBlock*>(cd->baseBlocks[startBlock2]);

                         qDebug() << "root2" << startBlock;
                         for(int i = 0; foundNext; i++){
                             qDebug() << dataB2->data;
                             dataEntr = dataEntr + dataB2->data;
                             if(dataB2->next != NULL){
                                 dataB2 = dataB2->next;
                             } else {
                                 foundNext = false;
                             }
                         }

                         vector<string> str = splitAuthor(dataEntr);
                         bool authorM  = false;
                         if(str[1].empty()){
                             str[1] = str[0];
                             authorM = true;
                             qDebug() << "overwrite";
                         }
                         if(entr->fileName != "root"){
                         qDebug() << "root4 -----------------------------" << str[1];
                         if(currentSystem == 1){
                             qDebug() << "root4 -----------------------------" <<entr->flags[1];
                             if(entr->flags[1] == false){
                                 qDebug() << "root4 -----------------------------";
                                 if(authorM){
                                     ISys->createFile(entr->fileName,"system",str[0],"root");
                                 } else {
                                     ISys->createFile(entr->fileName,str[1],str[1],"root");
                                 }
                             } else {
                                qDebug() << "rootsuper ----------------ssssssssssssssss-------------";
                                 string folderName = ISys->createFile(entr->fileName,str[0]," ","root",true);
                                 readFolder(cd,currentSystem,entr,folderName);


                             }

                             } else {
                             if(entr->flags[1] == false){
                                 qDebug() << "root4 -----------------------------";
                                 if(authorM){
                                     string name = FSys->createUniqueName(const_cast<char*>(entr->fileName.c_str()), "-D");
                                     FSys->createFile(const_cast<char*>(name.c_str()),"system",str[0],"root");
                                 } else {
                                     string name = FSys->createUniqueName(const_cast<char*>(entr->fileName.c_str()), "-D");
                                     FSys->createFile(const_cast<char*>(name.c_str()),const_cast<char*>(str[1].c_str()),str[1],"root");
                                 }
                             } else {
                                 qDebug() << "rootsuper ----------------ssssssssssssssss-------------";
                                 string name = FSys->createUniqueName(const_cast<char*>(entr->fileName.c_str()), "-D");
                                 char* folderName = FSys->createFile(const_cast<char*>(name.c_str()),const_cast<char*>(str[0].c_str())," ","root",true)->name;
                                 readFolder(cd,currentSystem,entr,folderName);


                             }

                            }
                         }

                     }

                 }
             qDebug() << "root4";
             foundPV = true;
             }

            }
        }
    }



void CDSYSTEM::readFolder(CDROM* cd , int currentSystem, FileEntry* folder, string folderName){

    qDebug() << "root //////////////";
    string data = "";
    bool foundNext = true;
    int startBlock = folder->startBlock;
    DataBlock *dataB = dynamic_cast<DataBlock*>(cd->baseBlocks[startBlock]);

    qDebug() << "root2" << startBlock;
    for(int i = 0; foundNext; i++){
        qDebug() << dataB->data;
        data = data + dataB->data;
        if(dataB->next != NULL){
            dataB = dataB->next;
        } else {
            foundNext = false;
        }
    }
    qDebug() << "root3" << data;

    bool authorM = false;
    vector<string> str = splitAuthor(data);
    qDebug() << "root 3" << str[0];
    if(str[1].empty()){
        str[1] = str[0];
        authorM = true;
        qDebug() << "overwrite";
    }
    vector<int> nums = splitBlocks(str[1]);
    qDebug() << "root 4" << nums.size();
    for(int i = 0; i < nums.size(); i++){


            FileEntry *entr =findEntry(cd,nums[i]);

            if(entr != nullptr){
             qDebug() << "root 5" << nums.size();




                    string dataEntr;
                    bool foundNext = true;
                    int startBlock2 = entr->startBlock;
                    DataBlock *dataB2 = dynamic_cast<DataBlock*>(cd->baseBlocks[startBlock2]);

                    qDebug() << "root2" << startBlock;
                    for(int i = 0; foundNext; i++){
                        qDebug() << dataB2->data;
                        dataEntr = dataEntr + dataB2->data;
                        if(dataB2->next != NULL){
                            dataB2 = dataB2->next;
                        } else {
                            foundNext = false;
                        }
                    }

                    vector<string> str = splitAuthor(dataEntr);
                    if(entr->fileName != "root"){
                        qDebug() << "root4 -----------------------------" << str[1];
                        if(currentSystem == 1){
                            qDebug() << "root4 -----------------------------" <<entr->flags[1];
                            if(entr->flags[1] == false){
                                qDebug() << "root4 -----------------------------";
                                if(authorM){
                                ISys->createFile(entr->fileName,"system",str[0],folderName);
                                } else {
                                     ISys->createFile(entr->fileName,str[1],str[1],folderName);
                                }
                            } else {
                                string folderName2 = ISys->createFile(entr->fileName,str[0]," ",folderName,true);
                                readFolder(cd,currentSystem,entr,folderName2);


                            }

                        } else {
                            if(entr->flags[1] == false){
                                qDebug() << "root4 -----------------------------";
                                if(authorM){
                                    string name = FSys->createUniqueName(const_cast<char*>(entr->fileName.c_str()), "-D");
                                    FSys->createFile(const_cast<char*>(name.c_str()),"system",str[0],const_cast<char*>(folderName.c_str()));
                                } else {
                                    string name = FSys->createUniqueName(const_cast<char*>(entr->fileName.c_str()), "-D");
                                    FSys->createFile(const_cast<char*>(name.c_str()),const_cast<char*>(str[1].c_str()),const_cast<char*>(str[1].c_str()),const_cast<char*>(folderName.c_str()));
                                }
                            } else {
                                string name = FSys->createUniqueName(const_cast<char*>(entr->fileName.c_str()), "-D");
                                string folderName2 = FSys->createFile(const_cast<char*>(name.c_str()),const_cast<char*>(str[1].c_str())," ",const_cast<char*>(folderName.c_str()),true)->name;
                                readFolder(cd,currentSystem,entr,folderName2);


                            }
                        }
                    }


            }

    }
}


FileEntry* CDSYSTEM::findEntry(CDROM* cd, int baseBlock){
    qDebug() << "***********";
    for(int i = 0; i < cd->baseBlocks.size(); i++){

        FileEntry *entr =nullptr;
        entr = dynamic_cast<FileEntry*>(cd->baseBlocks[i]);

        if(entr != nullptr){
            qDebug() << "*******************" << entr->fileName;
            if(entr->startBlock == baseBlock){
                qDebug() << "*******************" << entr->fileName;
                return entr;
            }
        }
    }
    return nullptr;
}

CDSYSTEM::CDSYSTEM(INODESYSTEM* Isys, FATSYSTEM* Fsys): ISys(Isys), FSys(Fsys) {}

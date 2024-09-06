#include "inodesystem.h"


#include "inodeSystem.h"


using namespace std;

#define BLOCKSIZE 512
#define MAX_FILES 10

map<int, INode *> INODESYSTEM::getNodes() const
{
    return nodes;
}

void INODESYSTEM::setNodes(const map<int, INode *> &newNodes)
{
    nodes = newNodes;
}

Disk *INODESYSTEM::getDisk() const
{
    return disk;
}

void INODESYSTEM::setDisk(Disk *newDisk)
{
    disk = newDisk;
}

INODESYSTEM::INODESYSTEM(int diskSpace, Disk *disk_): disk(disk_){

    totalBlocks = diskSpace/BLOCKSIZE;
    for (int i = 0; i < totalBlocks ; i++)
    {
        blockStatus.push_back(BLOCK_FREE2);
    }

    blockStatus[0] = BLOCK_RESERVED2;
    createFile("root", "system", " ", "isRoot", true);
    int ind = findFile("root");
    disk->addDataToBlock(0, to_string(ind));
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
    QList<int> list;
    for(int i = 0; i< nodes.size(); i++){
        if(nodes[i]!= NULL){

            list.append(nodes[i]->iNodeNum);
        }

    }
    bool search = true;

    while(search){
        if(list.contains(ind)){
            ind++;
        }else{
            search = false;
        }
    }
    return ind;
}


void INODESYSTEM::createFile(string name_, string author_, string data,string parentName, bool isFolder){ //add string parent
    int num =findLowestNumber();
    INode *node = new INode();
    node->name = name_;
    node->author = author_;

    node->date = QDateTime::currentDateTime();

    node->iNodeNum = num;
    nodes[num] = node;
    nodes[num]->name[11] = '\0';
    node->isFolder = isFolder;

    vector<string> dataChunks;
    size_t chunk_size = BLOCKSIZE;
    size_t length = data.length();
    int requiredBlocks = 0;
    for (size_t i = 0; i < length; i += chunk_size) {
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }
    node->size = length;
    //parent id
    int parentId = findFile(parentName);
    // parent in der liste finden     nodes[parentId];

    //id der erstellten file finden   num

    //parent daten anpassen, schauen ob mehr blöcke benötigt werden
    string parentData = "";
    int result = 0;
    if(parentName != "isRoot"){

    for(int i = 0; i < nodes[parentId]->blockList.size(); i++){
        parentData = parentData+disk->getBlocks()[nodes[parentId]->blockList[i]];
        }




    if(parentData == ""){

        result = editData(parentId, to_string(num));

    }else {

        string dataString = parentData+","+to_string(num);

        result = editData(parentId, dataString);

    }
    }
    if(result == 0){ // error handling
    //schauen ob genug platz da


    if (requiredBlocks > getFreeDiskSpace() / BLOCKSIZE) {
        editData(parentId, parentData);
        qDebug() <<"Nicht genug Platz" ;
        return;
    }

    int allocatedBlocks = 0;
    int pos = 0;
    for (int i = 0; i < totalBlocks && allocatedBlocks < requiredBlocks; i++) {
        if (blockStatus[i] == BLOCK_FREE2) {

            blockStatus[i] = BLOCK_OCCUPIED2;
            disk->addDataToBlock(i, dataChunks[pos]);
            pos++;
            allocatedBlocks++;
            nodes[num]->blockList.push_back(i);


        }
    }

    }
}

int INODESYSTEM::editData(int fileToBeEditedId, string data){
    //node select am index filestobeedited

    //wenn data mehr blocks benötigt

    vector<string> dataChunks;
    size_t chunk_size = BLOCKSIZE;
    size_t length = data.length();
    int requiredBlocks = 0;

    for (size_t i = 0; i < length; i += chunk_size) {
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }

    if(nodes[fileToBeEditedId]->blockList.size() < requiredBlocks){

        if (requiredBlocks > getFreeDiskSpace() / BLOCKSIZE) {
            qDebug() <<"Nicht genug Platz" ;
            return -1;
        }

        requiredBlocks = requiredBlocks - nodes[fileToBeEditedId]->blockList.size();
        int allocatedBlocks = 0;
        int pos = 0;
        for (int i = 0; i < totalBlocks && allocatedBlocks < requiredBlocks; i++) {
            if (blockStatus[i] == BLOCK_FREE2) {

                blockStatus[i] = BLOCK_OCCUPIED2;

                pos++;
                allocatedBlocks++;
                nodes[allocatedBlocks]->blockList.push_back(i);


            }
        }
    }


    for(int i = 0; i < nodes[fileToBeEditedId]->blockList.size(); i++){
        disk->addDataToBlock(nodes[fileToBeEditedId]->blockList[i], dataChunks[i]);
    }
    return 0;
}

int INODESYSTEM::findFile(int num) {
    if (nodes.find(num) != nodes.end()) {
        return num;
    }
    cout << "cant find File";
    return NULL;
}
int INODESYSTEM::findFile( string fileName){
    for (int i = 0; i < MAX_FILES; i++) {
        if (nodes[i] != NULL && nodes[i]->name == fileName) {
            return i;
        }
    }
    cout << "cant find File";
    return -1;
}

void INODESYSTEM::deleteFile( string fileName){
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







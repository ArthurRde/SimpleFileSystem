#include "inodesystem.h"


#include "inodeSystem.h"


using namespace std;

#define BLOCKSIZE 512


map<int, INode *> INODESYSTEM::getNodes() const {
    return nodes;
}

void INODESYSTEM::setNodes(const map<int, INode *> &newNodes) {
    nodes = newNodes;
}

Disk *INODESYSTEM::getDisk() const {
    return disk;
}

void INODESYSTEM::setDisk(Disk *newDisk) {
    disk = newDisk;
}


INODESYSTEM::INODESYSTEM(int diskSpace, Disk *disk_) : disk(disk_) {
    totalBlocks = diskSpace / BLOCKSIZE;
    for (int i = 0; i < totalBlocks; i++) {
        blockStatus.push_back(BLOCK_FREE);
    }

    blockStatus[0] = BLOCK_RESERVED;
    createFile("root", "system", " ", "isRoot", true);
    int ind = findFile("root");
    disk->addDataToBlock(0, to_string(ind));
}

int INODESYSTEM::getFreeDiskSpace() {
    int freeBlocks = 0;
    for (int i = 0; i < totalBlocks; i++) {
        if (blockStatus[i] == BLOCK_FREE) {
            freeBlocks++;
        }
    }
    return freeBlocks * BLOCKSIZE;
}

int INODESYSTEM::findLowestNumber() {
    int ind = 0;
    QList<int> list;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i] != NULL) {

            list.append(nodes[i]->iNodeNum);
        }

    }
    bool search = true;

    while (search) {
        if (list.contains(ind)) {
            ind++;
        } else {
            search = false;
        }
    }
    return ind;
}


bool INODESYSTEM::existFileInFolder(string fileName, string folderName) {
    bool ex = false;
    QList < INode * > node = getFilesInFolder(folderName);
    for (int i = 0; i < node.size(); i++) {
        if (node[i]->name == fileName) {
            ex = true;
        }
    }
    return ex;
}

bool INODESYSTEM::existFile(string fileName) {
    bool ex = false;

    for (int i = 0; i < getNodes().size(); i++) {

        if (getNodes()[i] != NULL) {
            if (getNodes()[i]->name == fileName) {
                ex = true;
            }
        }

    }

    return ex;
}


string INODESYSTEM::createFile(string name_, string author_, string data, string parentName,
                               bool isFolder) { //add string parent
    int num = findLowestNumber();
    INode *node = new INode();
    node->name = name_;
    if (parentName != "isRoot") {
        if (existFile(name_)) {
            node->name = createUniqueName(name_, "-D");
        }
    }
    node->author = author_;

    node->date = QDateTime::currentDateTime();

    node->iNodeNum = num;
    nodes[num] = node;
    nodes[num]->name[31] = '\0';
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
    int parentId = findFile(parentName);
    string parentData = "";
    int result = 0;
    if (parentName != "isRoot") {

        for (int i = 0; i < nodes[parentId]->blockList.size(); i++) {
            parentData = parentData + disk->getBlocks()[nodes[parentId]->blockList[i]];
        }


        if (parentData.empty()) {

            result = editData(parentId, to_string(num));

        } else {

            string dataString = parentData + "," + to_string(num);

            result = editData(parentId, dataString);

        }
    }
    if (result == 0) { // error handling
        //schauen ob genug platz da


        if (requiredBlocks > getFreeDiskSpace() / BLOCKSIZE) {
            editData(parentId, parentData);
            qDebug() << "Nicht genug Platz";
            return "";
        }

        int allocatedBlocks = 0;
        int pos = 0;
        for (int i = 0; i < totalBlocks && allocatedBlocks < requiredBlocks; i++) {
            if (blockStatus[i] == BLOCK_FREE) {

                blockStatus[i] = BLOCK_OCCUPIED;
                disk->addDataToBlock(i, dataChunks[pos]);
                pos++;
                allocatedBlocks++;
                nodes[num]->blockList.push_back(i);


            }
        }

    }

    return node->name;
}

int INODESYSTEM::editData(int fileToBeEditedId, string data) {
    bool isEmpty = false;
    if (data.empty()) {
        isEmpty = true;
    }
    vector<string> dataChunks;
    size_t chunk_size = BLOCKSIZE;
    size_t length = data.length();
    int requiredBlocks = 0;


    for (size_t i = 0; i < length; i += chunk_size) {
        dataChunks.push_back(data.substr(i, chunk_size));
        requiredBlocks++;
    }


    if (nodes[fileToBeEditedId]->blockList.size() < requiredBlocks) {

        if (requiredBlocks > getFreeDiskSpace() / BLOCKSIZE) {
            qDebug() << "Nicht genug Platz";
            return -1;

        }
        nodes[fileToBeEditedId]->size = length;
        requiredBlocks = requiredBlocks - nodes[fileToBeEditedId]->blockList.size();
        int allocatedBlocks = 0;
        int pos = 0;
        for (int i = 0; i < totalBlocks && allocatedBlocks < requiredBlocks; i++) {
            if (blockStatus[i] == BLOCK_FREE) {

                blockStatus[i] = BLOCK_OCCUPIED;
                pos++;
                allocatedBlocks++;
                nodes[fileToBeEditedId]->blockList.push_back(i);


            }
        }
    }

    int size = nodes[fileToBeEditedId]->blockList.size();
    for (int i = 0; i < size; i++) {
        if (!isEmpty) {
            disk->addDataToBlock(nodes[fileToBeEditedId]->blockList[i], dataChunks[i]);
        } else {
            disk->addDataToBlock(nodes[fileToBeEditedId]->blockList[i], " ");
        }

    }
    return 0;
}

int INODESYSTEM::findFile(int num) {
    if (nodes.find(num) != nodes.end()) {
        return num;
    }
    return NULL;
}

int INODESYSTEM::findFile(string fileName) {
    for (int i = 0; i < getNodes().size(); i++) {
        if (nodes[i] != NULL && nodes[i]->name == fileName) {
            return i;
        }
    }
    cout << "cant find File inode";
    return -1;
}

string INODESYSTEM::findFolderofFile(string fileName) {
    for (int i = 0; i < getNodes().size(); i++) {
        if (getNodes()[i] != NULL) {
            if (getNodes()[i]->isFolder) {
                QList < INode * > nodes = getFilesInFolder(getNodes()[i]->name);
                if (nodes.contains(getNodes()[findFile(fileName)])) {
                    return getNodes()[i]->name;
                }
            }
        }
    }
    return "";
}

void INODESYSTEM::deleteFile(string fileName, bool deleteFolderInFolder, bool ignoreFolderTyp) {
    int num = findFile(fileName);

    if ((!getNodes()[num]->isFolder) || ignoreFolderTyp) {
        if (num != -1) {
            for (int i = 0; i < nodes[num]->blockList.size(); i++) {
                blockStatus[nodes[num]->blockList[i]] = BLOCK_FREE;
                disk->getBlocks()[nodes[num]->blockList[i]] = " ";
            }
            string folderName = findFolderofFile(fileName);
            int folderId = findFile(folderName);
            string folderData = "";
            for (int i = 0; i < nodes[folderId]->blockList.size(); i++) {
                folderData = folderData + disk->getBlocks()[nodes[folderId]->blockList[i]];
            }
            string newFolderData = "";
            vector<int> folderNums = splitStringIntoInts(folderData);
            for (int i = 0; i < folderNums.size(); i++) {
                if (folderNums[i] != num) {
                    newFolderData = newFolderData + ", " + to_string(folderNums[i]);
                }
            }
            editData(folderId, newFolderData);
            INode *nodeToDelete = nodes[num];
            nodes.erase(num);
            delete nodeToDelete;

        }
    } else {

        if (!deleteFolderInFolder) {

            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);

            msgBox.setWindowTitle("Attention");
            msgBox.setText("You try to delete a folder! Do you want to continue?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            if (msgBox.exec() == QMessageBox::Yes) {
                deleteFolder(fileName);

            }

        } else {
            deleteFolder(fileName);
        }
    }
}


void INODESYSTEM::deleteFolder(string fileName) {
    QList < INode * > node = getFilesInFolder(fileName);

    for (int i = 0; i < node.size(); i++) {
        if (!node[i]->isFolder) {
            deleteFile(node[i]->name);
        } else {
            deleteFile(node[i]->name, true);
        }
    }
    deleteFile(fileName, false, true);
}

void INODESYSTEM::showFat() {
    for (int i = 0; i < totalBlocks; i++) {
        switch (blockStatus[i]) {
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

    cout << endl;
}

void INODESYSTEM::renameFile(string fileName, string newName) {

    int id = findFile(fileName);

    nodes[id]->name = newName;
}


QList<INode *> INODESYSTEM::getFoldersInFolder(string folderName) {
    INode *folder = getNodes()[findFile(folderName)];
    string data;

    for (int i = 0; i < folder->blockList.size(); i++) {
        data = data + getDisk()->getBlocks()[folder->blockList[i]];
    }
    vector<int> nums = splitStringIntoInts(data);
    QList < INode * > node;
    for (int i = 0; i < nums.size(); i++) {
        if (getNodes()[nums[i]]->isFolder) {
            node.append(getNodes()[nums[i]]);
        }
    }
    return node;
}

QList<INode *> INODESYSTEM::getFilesInFolder(string folderName) {
    INode *folder = getNodes()[findFile(folderName)];

    string data;
    for (int i = 0; i < folder->blockList.size(); i++) {

        data = data + getDisk()->getBlocks()[folder->blockList[i]];
    }

    vector<int> nums = splitStringIntoInts(data);

    QList < INode * > node;
    for (int i = 0; i < nums.size(); i++) {
        node.append(getNodes()[nums[i]]);
    }
    return node;
}


vector<int> INODESYSTEM::splitStringIntoInts(string inputString) {
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


string INODESYSTEM::getDataOfFile(string fileName) {
    int num = findFile(fileName);
    string data = "";
    for (int i = 0; i < getNodes()[num]->blockList.size(); i++) {
        data = data + disk->getBlocks()[getNodes()[num]->blockList[i]];
    }
    return data;
}

string INODESYSTEM::createUniqueName(string fileName, string uniqueText) {
    string name = fileName;
    bool ex = existFile(name);
    while (ex) {

        stringstream ss(name);
        string token;
        vector<string> tokens;
        char delimiter = '.';
        if (name != " ") {
            while (getline(ss, token, delimiter)) {
                if ((token != " ")) {
                    if (!token.empty()) {
                        tokens.push_back(token);

                    }
                }
            }
        }
        tokens[0] = tokens[0] + uniqueText;
        name = tokens[0];
        for (int i = 1; i < tokens.size(); i++) {
            name = name + "." + tokens[i];
        }
        ex = existFile(name);
    }
    return name;
}

void INODESYSTEM::copyFile(string fileName, string folderName) {
    int fileNum = findFile(fileName);
    string name = createUniqueName(getNodes()[fileNum]->name);

    if (!getNodes()[fileNum]->isFolder) {
        bool ex = existFile(getNodes()[fileNum]->name);
        createFile(name, getNodes()[fileNum]->author, getDataOfFile(getNodes()[fileNum]->name), folderName);
    } else {
        createFile(name, getNodes()[fileNum]->author, " ", folderName, true);
        editData(findFile(name), " ");
        vector<int> nums = splitStringIntoInts(getDataOfFile(getNodes()[fileNum]->name));
        for (int i = 0; i < nums.size(); i++) {

        }
        for (int i = 0; i < nums.size(); i++) {
            copyFile(getNodes()[nums[i]]->name, getNodes()[findFile(name)]->name);
        }


    }
}

float INODESYSTEM::getFragmentation() {
    int fragmentedBlocks = 0;

    for (int i = 0; i < blockStatus.size() - 1; i++) {
        if (blockStatus[i] == BLOCK_OCCUPIED) {
            if (blockStatus[i + 1] == BLOCK_FREE) {
                fragmentedBlocks++;
            } else if (blockStatus[i + 1] == BLOCK_OCCUPIED) {
                int fileNum = 0;
                for (int j = 0; j < nodes.size(); j++) {
                    if (nodes[j] != nullptr) {
                        vector<int> blocklist = nodes[j]->blockList;
                        for (int k = 0; k < blocklist.size(); k++) {
                            if (blocklist[k] == i) {
                                fileNum = j;
                                break;
                            }
                        }
                    }
                }
                int nextFileNum = 0;
                for (int j = 0; j < nodes.size(); j++) {
                    if (nodes[j] != nullptr) {
                        vector<int> blocklist = nodes[j]->blockList;
                        for (int k = 0; k < blocklist.size(); k++) {
                            if (blocklist[k] == i + 1) {
                                nextFileNum = j;
                                break;
                            }
                        }
                    }
                }
                if (nextFileNum != fileNum) {
                    fragmentedBlocks++;
                }
            }
        } else if (blockStatus[i] == BLOCK_FREE) {
            if (blockStatus[i + 1] == BLOCK_OCCUPIED) {
                fragmentedBlocks++;
            }
        }
    }

    if ((blockStatus[totalBlocks - 2] == BLOCK_OCCUPIED && blockStatus[totalBlocks - 1] == BLOCK_FREE) ||
        (blockStatus[totalBlocks - 1] == BLOCK_OCCUPIED && blockStatus[totalBlocks - 2] == BLOCK_FREE)) {
        fragmentedBlocks++;
    } else {
        int fileNum = 0;
        for (int j = 0; j < nodes.size(); j++) {
            if (nodes[j] != nullptr) {
                vector<int> blocklist = nodes[j]->blockList;
                for (int k = 0; k < blocklist.size(); k++) {
                    if (blocklist[k] == totalBlocks - 1) {
                        fileNum = j;
                        break;
                    }
                }
            }
        }
        int nextFileNum = 0;
        for (int j = 0; j < nodes.size(); j++) {
            if (nodes[j] != nullptr) {
                vector<int> blocklist = nodes[j]->blockList;
                for (int k = 0; k < blocklist.size(); k++) {
                    if (blocklist[k] == totalBlocks) {
                        nextFileNum = j;
                        break;
                    }
                }
            }
        }
        if (nextFileNum != fileNum) {
            fragmentedBlocks++;
        }
    }

    return ((float) fragmentedBlocks / (float) totalBlocks) * 100.0f;
}

void INODESYSTEM::defragDisk() {
    cout << "Starting defragmentation..." << endl;

    for (int iteration = 0; iteration < totalBlocks; iteration++) {
        int blockIndex = 0;
        for (int fileIdx = 0; fileIdx < nodes.size(); fileIdx++) {
            if (nodes[fileIdx] != nullptr) {
                vector<int> blocklist = nodes[fileIdx]->blockList;

                for (int i = 0; i < blocklist.size(); i++) {
                    while (blockIndex < totalBlocks && blockStatus[blockIndex] != BLOCK_FREE) {
                        blockIndex++;
                    }

                    if (blockIndex < totalBlocks) {
                        blockStatus[blocklist[i]] = BLOCK_FREE;
                        blocklist[i] = blockIndex;
                        blockStatus[blockIndex] = BLOCK_OCCUPIED;
                        blockIndex++;
                    }
                }
            }
        }
    }

    cout << "Defragmentation completed." << endl;
}

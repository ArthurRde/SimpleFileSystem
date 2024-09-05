#ifndef DISK_H
#define DISK_H

#include <iostream>
#include <vector>

using namespace std;
class Disk
{
    vector<string> blocks;

public:
    Disk(int numberOfBlocks);
    void addDataToBlock(int blockNum, string data_);
    vector<string> getBlocks() const;
    void setBlocks(const vector<string> &newBlocks);
};

#endif // DISK_H

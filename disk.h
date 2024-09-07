#ifndef DISKH
#define DISK_H

#pragma once
#include <iostream>
#include <vector>

using namespace std;
class Disk
{
    vector<string> blocks;
    int size;
    int blockSize;

public:
    Disk(int numberOfBlocks, int blockSize_);
    void addDataToBlock(int blockNum, string data_);
    vector<string> getBlocks() const;
    void setBlocks(const vector<string> &newBlocks);
    int getSize() const;
    void setSize(int newSize);
    int getBlockSize() const;
    void setBlockSize(int newBlockSize);
};

#endif // DISK_H

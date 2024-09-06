#include "disk.h"

vector<string> Disk::getBlocks() const
{
    return blocks;
}

void Disk::setBlocks(const vector<string> &newBlocks)
{
    blocks = newBlocks;
}

int Disk::getSize() const
{
    return size;
}

void Disk::setSize(int newSize)
{
    size = newSize;
}

int Disk::getBlockSize() const
{
    return blockSize;
}

void Disk::setBlockSize(int newBlockSize)
{
    blockSize = newBlockSize;
}

Disk::Disk(int numberOfBlocks, int blockSize_) {

    blockSize = blockSize_;
    size = numberOfBlocks * blockSize;
    for(int i = 0; i < numberOfBlocks; i++){
        blocks.push_back("");
    }

}

void Disk::addDataToBlock(int blockNum, string data_)
{
    blocks[blockNum] = data_;
}

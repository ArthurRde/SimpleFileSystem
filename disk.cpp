#include "disk.h"

vector<string> Disk::getBlocks() const
{
    return blocks;
}

void Disk::setBlocks(const vector<string> &newBlocks)
{
    blocks = newBlocks;
}

Disk::Disk(int numberOfBlocks) {


    for(int i = 0; i < numberOfBlocks; i++){
        blocks.push_back("");
    }

}

void Disk::addDataToBlock(int blockNum, string data_)
{
    blocks[blockNum] = data_;
}

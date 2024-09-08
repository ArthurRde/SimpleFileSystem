#ifndef DISKH
#define DISK_H

#pragma once

#include <iostream>
#include <vector>

using namespace std;

/**
 * @class Disk
 * @brief Represents a disk with blocks of data.
 */
class Disk {
    vector<string> blocks; /**< Vector of blocks containing data */
    int size; /**< Size of the disk */
    int blockSize; /**< Size of each block */

public:
    /**
     * @brief Constructor for Disk.
     * @param numberOfBlocks Number of blocks in the disk
     * @param blockSize_ Size of each block
     */
    Disk(int numberOfBlocks, int blockSize_);

    /**
     * @brief Adds data to a specific block.
     * @param blockNum Index of the block
     * @param data_ Data to add to the block
     */
    void addDataToBlock(int blockNum, string data_);

    /**
     * @brief Gets the blocks of the disk.
     * @return Vector of blocks
     */
    vector<string> getBlocks() const;

    /**
     * @brief Sets the blocks of the disk.
     * @param newBlocks Vector of new blocks
     */
    void setBlocks(const vector<string> &newBlocks);

    /**
     * @brief Gets the size of the disk.
     * @return Size of the disk
     */
    int getSize() const;

    /**
     * @brief Sets the size of the disk.
     * @param newSize New size of the disk
     */
    void setSize(int newSize);

    /**
     * @brief Gets the size of each block.
     * @return Size of each block
     */
    int getBlockSize() const;

    /**
     * @brief Sets the size of each block.
     * @param newBlockSize New size of each block
     */
    void setBlockSize(int newBlockSize);
};

#endif // DISK_H
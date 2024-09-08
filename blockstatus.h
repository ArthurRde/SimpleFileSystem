#ifndef BLOCKSTATUS_H
#define BLOCKSTATUS_H

#pragma once

/**
 * @brief Enum representing the status of a block on the disk.
 *
 * This enum defines the possible states that a block on the disk can have.
 */
enum BlockStatus {
    BLOCK_RESERVED, /**< Block is reserved and cannot be used. */
    BLOCK_DEFECT,   /**< Block is defective and unusable. */
    BLOCK_FREE,     /**< Block is available for use. */
    BLOCK_OCCUPIED  /**< Block is currently occupied by data. */
};

/**
 * @brief The blockStatus class represents a block status management system.
 *
 * This class provides functionality to manage and track the status of blocks on the disk.
 */
class blockStatus {
public:
    /**
     * @brief Constructor for the blockStatus class.
     *
     * Initializes the block status management system.
     */
    blockStatus();
};

#endif // BLOCKSTATUS_H
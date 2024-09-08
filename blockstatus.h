#ifndef BLOCKSTATUS_H
#define BLOCKSTATUS_H

#pragma once

enum BlockStatus {
    BLOCK_RESERVED, /**< Block is reserved */
    BLOCK_DEFECT,   /**< Block is defective */
    BLOCK_FREE,     /**< Block is free */
    BLOCK_OCCUPIED  /**< Block is occupied */
};

class blockStatus {
public:
    blockStatus();
};

#endif // BLOCKSTATUS_H
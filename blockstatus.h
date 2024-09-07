#ifndef BLOCKSTATUS_H
#define BLOCKSTATUS_H

#pragma once
enum BlockStatus {
    BLOCK_RESERVED,
    BLOCK_DEFECT,
    BLOCK_FREE,
    BLOCK_OCCUPIED
};
class blockStatus
{
public:
    blockStatus();
};

#endif // BLOCKSTATUS_H

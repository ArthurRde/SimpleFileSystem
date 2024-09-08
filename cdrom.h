#ifndef CDROM_H
#define CDROM_H

#include "QtCore/qdatetime.h"

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <QDebug>
#include "blockstatus.h"

using namespace std;

/**
 * @brief Abstract base class for representing entries.
 */
struct Base {
    /**
     * @brief Virtual destructor for the Base class.
     */
    virtual ~Base() = default;

    /**
     * @brief Pure virtual function to print the details of the entry.
     */
    virtual void print() const = 0;
};

/**
 * @brief Struct representing a file entry in the system.
 */
struct FileEntry : Base {
    uint8_t entryLength = 1;              ///< The length of the file entry.
    uint8_t extendedAttributeLength;      ///< Length of the extended attributes.

    uint64_t startBlock;                  ///< The starting block of the file.
    uint64_t fileSize;                    ///< The size of the file.
    char dateTime[7];                     ///< The date and time information.

    char *flags = new char[3];            ///< Flags associated with the file entry.
    uint8_t fileNameLength;               ///< The length of the file name.
    string fileName;                      ///< The name of the file.
    vector<uint8_t> padding;              ///< Padding to align data.

    /**
     * @brief Constructor to initialize a file entry.
     * @param name Name of the file.
     * @param size Size of the file.
     */
    FileEntry(const string &name, uint64_t size) : fileName(name), fileSize(size) {
        extendedAttributeLength = 0;
        time_t t = time(NULL);
        tm *now = gmtime(&t);

        dateTime[0] = now->tm_year % 100;
        dateTime[1] = now->tm_mon + 1;
        dateTime[2] = now->tm_mday;
        dateTime[3] = now->tm_hour;
        dateTime[4] = now->tm_min;
        dateTime[5] = now->tm_sec;
        dateTime[6] = 0;

        fileNameLength = fileName.size();
    }

    /**
     * @brief Print the file entry details.
     */
    void print() const override {
        qDebug() << "";
    }
};

/**
 * @brief Struct representing a primary volume descriptor.
 */
struct PrimaryVolumeDescriptor : Base {
    char systemIdentifier[32] = "SYSTEM";           ///< Identifier for the system.
    char volumeIdentifier[32] = "VOL1";             ///< Identifier for the volume.
    char publisherIdentifier[128] = "CD PUBLISH";   ///< Identifier for the publisher.
    char preparerIdentifier[128] = "IDENTIFIED";    ///< Identifier for the preparer.

    uint16_t logicalBlockSize;                      ///< Logical block size of the volume.
    uint32_t volumeSpaceSize;                       ///< Total size of the volume in blocks.
    char creationDate[17];                          ///< The creation date of the volume.
    char expirationDate[17];                        ///< The expiration date of the volume.
    FileEntry *rootDirectoryEntry;                  ///< Root directory entry.

    string summaryFileName;                         ///< Summary file name.
    string copyrightFileName;                       ///< Copyright file name.
    string bibliographicFileName;                   ///< Bibliographic file name.

    /**
     * @brief Constructor for the PrimaryVolumeDescriptor.
     * @param root Pointer to the root directory entry.
     */
    PrimaryVolumeDescriptor(FileEntry *root)
            : logicalBlockSize(2352), volumeSpaceSize(0), rootDirectoryEntry(root), summaryFileName("test"),
              copyrightFileName("test"), bibliographicFileName("test") {
        time_t t = time(NULL);
        tm *now = gmtime(&t);

        creationDate[0] = now->tm_year % 100;
        creationDate[1] = now->tm_mon + 1;
        creationDate[2] = now->tm_mday;
        creationDate[3] = now->tm_hour;
        creationDate[4] = now->tm_min;
        creationDate[5] = now->tm_sec;
        creationDate[6] = 0;

        expirationDate[0] = now->tm_year % 100 + 10;
        expirationDate[1] = now->tm_mon + 1;
        expirationDate[2] = now->tm_mday;
        expirationDate[3] = now->tm_hour;
        expirationDate[4] = now->tm_min;
        expirationDate[5] = now->tm_sec;
        expirationDate[6] = 0;

        fill(systemIdentifier, systemIdentifier + 32, ' ');
        fill(volumeIdentifier, volumeIdentifier + 32, ' ');
        fill(publisherIdentifier, publisherIdentifier + 128, ' ');
        fill(preparerIdentifier, preparerIdentifier + 128, ' ');

        t = time(NULL);
        now = gmtime(&t);
        strftime(creationDate, 17, "%Y%m%d%H%M%S00", now);
        strftime(expirationDate, 17, "%Y%m%d%H%M%S00", now);
    }

    /**
     * @brief Print the primary volume descriptor details.
     */
    void print() const override {
        qDebug() << "";
    }
};

/**
 * @brief Struct representing a data block in the CD-ROM.
 */
struct DataBlock : Base {
    string data;   ///< Data stored in the block.

    /**
     * @brief Constructor to initialize a data block.
     * @param data_ The data to be stored.
     */
    DataBlock(string data_) : data(data_) {}

    DataBlock *next = NULL; ///< Pointer to the next data block.

    /**
     * @brief Print the data block details.
     */
    void print() const override {
        qDebug() << "";
    }
};

/**
 * @brief Class representing a CD-ROM with various blocks and statuses.
 */
class CDROM {
    bool isBurned = false;   ///< Indicates whether the CD-ROM is burned or not.
public:
    int blockSize = 2352;         ///< Size of a block on the CD-ROM.
    int blockPortion = 2048;      ///< Portion of the block that contains useful data.
    int diskSize = 235200;        ///< Total size of the disk in blocks.
    vector<Base *> baseBlocks;    ///< Vector of base blocks.
    vector<BlockStatus> baseStatus; ///< Vector of block statuses.

    string portName;   ///< Name of the port to which the CD-ROM is connected.

    /**
     * @brief Constructor for the CDROM class.
     * @param portName_ The name of the port to which the CD-ROM is connected.
     */
    CDROM(string portName_);

    /**
     * @brief Get the burn status of the CD-ROM.
     * @return true if the CD-ROM is burned, false otherwise.
     */
    bool getIsBurned() const;

    /**
     * @brief Set the burn status of the CD-ROM.
     * @param newIsBurned The new burn status.
     */
    void setIsBurned(bool newIsBurned);
};

#endif // CDROM_H

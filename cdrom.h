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

struct Base {
    virtual ~Base() = default;

    virtual void print() const = 0;
};

//Struct für ein File
struct FileEntry : Base {
    uint8_t entryLength = 1;
    uint8_t extendedAttributeLength;

    uint64_t startBlock;
    uint64_t fileSize;
    char dateTime[7]; //ist hier QDate besser? das ist aber so mehr bei der Byteanzahl


    char *flags = new char[3];
    uint8_t fileNameLength;
    string fileName;
    vector<uint8_t> padding;

    FileEntry(const string &name, uint64_t size) : fileName(name), fileSize(size) {

        // uint64_t start
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
        //padding.resize((entryLength % 2 == 0) ? 0 : 1, 0);
    }

    void print() const override {
        qDebug() << "";
    }
};

struct PrimaryVolumeDescriptor : Base {
    char systemIdentifier[32] = "SYSTEM";
    char volumeIdentifier[32] = "VOL1";
    char publisherIdentifier[128] = "CD PUBLISH";

    char preparerIdentifier[128] = "IDENTIFIED";

    uint16_t logicalBlockSize;
    uint32_t volumeSpaceSize;
    char creationDate[17];
    char expirationDate[17];
    FileEntry *rootDirectoryEntry;

    string summaryFileName;
    string copyrightFileName;
    string bibliographicFileName;

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


    void print() const override {
        qDebug() << "";
    }
};

struct DataBlock : Base {
    string data;

    DataBlock(string data_) : data(data_) {

    };

    DataBlock *next = NULL;

    void print() const override {
        qDebug() << "";
    }
};


class CDROM {
    bool isBurned = false;
public:
    CDROM(string portName_);

    int blockSize = 2352;
    int blockPortion = 2048;
    int diskSize = 235200;
    vector<Base *> baseBlocks;
    vector<BlockStatus> baseStatus;


    string portName;

    bool getIsBurned() const;

    void setIsBurned(bool newIsBurned);
};

#endif // CDROM_H

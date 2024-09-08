#ifndef INODESYSTEM_H
#define INODESYSTEM_H

#include <vector>
#include <map>
#include <QDate>
#include "disk.h"
#include <string>
#include <string.h>
#include <sstream>
#include <QMessageBox>
#include "blockstatus.h"

using namespace std;

#define BLOCKSIZE 512
#define MAX_FILES 10

/**
 * @struct INode
 * @brief Represents an inode in the file system.
 */
struct INode {
    int iNodeNum;               /**< Inode number */
    string name;                /**< Name of the file or folder */
    vector<int> blockList;      /**< List of blocks associated with the inode */
    string author;              /**< Author of the file or folder */
    QDateTime date;             /**< Date of creation or modification */
    int size;                   /**< Size of the file or folder */
    bool isFolder;              /**< Flag indicating if the inode is a folder */
};

/**
 * @class INODESYSTEM
 * @brief Manages inodes and file operations in the file system.
 */
class INODESYSTEM {
    map<int, INode *> nodes;    /**< Map of inode numbers to inode pointers */
    Disk *disk;                 /**< Pointer to the disk */
    vector<BlockStatus> blockStatus; /**< Vector of block statuses */
    int totalBlocks;            /**< Total number of blocks in the disk */

public:
    /**
     * @brief Constructor for INODESYSTEM.
     * @param diskSize Size of the disk.
     * @param disk_ Pointer to the disk.
     */
    INODESYSTEM(int diskSize, Disk *disk_);

    /**
     * @brief Displays the file allocation table (FAT).
     */
    void showINodes();

    /**
     * @brief Deletes a file from the file system.
     * @param fileName Name of the file to delete.
     * @param deleteFolderInFolder Flag to delete folders within folders.
     * @param ignoreFolderTyp Flag to ignore folder type.
     */
    void deleteFile(string fileName, bool deleteFolderInFolder = false, bool ignoreFolderTyp = false);

    /**
     * @brief Finds a file by name.
     * @param fileName Name of the file to find.
     * @return Inode number of the file.
     */
    int findFile(string fileName);

    /**
     * @brief Finds a file by inode number.
     * @param num Inode number of the file to find.
     * @return Inode number of the file.
     */
    int findFile(int num);

    /**
     * @brief Creates a file in the file system.
     * @param name_ Name of the file.
     * @param author_ Author of the file.
     * @param data Data to be stored in the file.
     * @param parentName Name of the parent folder.
     * @param isFolder Flag indicating if the file is a folder.
     * @return Name of the created file.
     */
    string createFile(string name_, string author_, string data, string parentName, bool isFolder = false);

    /**
     * @brief Finds the lowest available inode number.
     * @return Lowest available inode number.
     */
    int findLowestNumber();

    /**
     * @brief Finds the second lowest available inode number.
     * @return Second lowest available inode number.
     */
    int findLowestNumber2();

    /**
     * @brief Gets the free disk space.
     * @return Free disk space.
     */
    int getFreeDiskSpace();

    /**
     * @brief Renames a file in the file system.
     * @param fileName Current name of the file.
     * @param newName New name of the file.
     */
    void renameFile(string fileName, string newName);

    /**
     * @brief Gets the nodes in the file system.
     * @return Map of inode numbers to inode pointers.
     */
    map<int, INode *> getNodes() const;

    /**
     * @brief Sets the nodes in the file system.
     * @param newNodes Map of inode numbers to inode pointers.
     */
    void setNodes(const map<int, INode *> &newNodes);

    /**
     * @brief Gets the disk.
     * @return Pointer to the disk.
     */
    Disk *getDisk() const;

    /**
     * @brief Sets the disk.
     * @param newDisk Pointer to the new disk.
     */
    void setDisk(Disk *newDisk);

    /**
     * @brief Gets the fragmentation of the disk.
     * @return Fragmentation percentage.
     */
    float getFragmentation();

    /**
     * @brief Defragments the disk.
     */
    void defragDisk();

    /**
     * @brief Edits the data of a file.
     * @param fileToBeEditedId Inode number of the file to be edited.
     * @param data New data to be stored in the file.
     * @return Status of the edit operation.
     */
    int editData(int fileToBeEditedId, string data);

    /**
     * @brief Finds the folder containing a file.
     * @param fileName Name of the file.
     * @return Name of the folder containing the file.
     */
    string findFolderofFile(string fileName);

    /**
     * @brief Gets the folders in a folder.
     * @param folderName Name of the folder.
     * @return List of inodes representing the folders.
     */
    QList<INode *> getFoldersInFolder(string folderName);

    /**
     * @brief Gets the files in a folder.
     * @param folderName Name of the folder.
     * @return List of inodes representing the files.
     */
    QList<INode *> getFilesInFolder(string folderName);

    /**
     * @brief Splits a string into a vector of integers.
     * @param inputString String to be split.
     * @return Vector of integers.
     */
    vector<int> splitStringIntoInts(string inputString);

    /**
     * @brief Deletes a folder from the file system.
     * @param fileName Name of the folder to delete.
     */
    void deleteFolder(string fileName);

    /**
     * @brief Checks if a file exists in a folder.
     * @param fileName Name of the file.
     * @param folderName Name of the folder.
     * @return true if the file exists, false otherwise.
     */
    bool existFileInFolder(string fileName, string folderName);

    /**
     * @brief Gets the data of a file.
     * @param fileName Name of the file.
     * @return Data of the file.
     */
    string getDataOfFile(string fileName);

    /**
     * @brief Creates a unique name for a file.
     * @param fileName Base name of the file.
     * @param uniqueText Text to make the name unique.
     * @return Unique name of the file.
     */
    string createUniqueName(string fileName, string uniqueText = "-C");

    /**
     * @brief Copies a file to a folder.
     * @param fileName Name of the file to copy.
     * @param folderName Name of the folder to copy the file to.
     */
    void copyFile(string fileName, string folderName);

    /**
     * @brief Checks if a file exists in the file system.
     * @param fileName Name of the file.
     * @return true if the file exists, false otherwise.
     */
    bool existFile(string fileName);
};

#endif // INODESYSTEM_H

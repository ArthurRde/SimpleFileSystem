#ifndef FATSYSTEM_H
#define FATSYSTEM_H

#define MAX_FILES 10

#include <vector>
#include "disk.h"
#include <QDateTime>
#include "blockstatus.h"
#include <string.h>
#include <sstream>
#include <QMessageBox>

using namespace std;

/**
 * @struct Cluster
 * @brief Represents a cluster in the FAT system. Each cluster points to the next one, forming a chain of clusters for a file.
 */
struct Cluster {
    int blockIndex;          ///< Index of the block on the disk
    struct Cluster *next;    ///< Pointer to the next cluster in the chain
    struct Cluster *prev;    ///< Pointer to the previous cluster in the chain
};

/**
 * @struct File
 * @brief Represents a file or folder in the FAT system.
 */
struct File {
    char name[32];           ///< Name of the file
    int size;                ///< Size of the file in bytes
    char author[32];         ///< Author of the file
    QDateTime date;          ///< Creation date of the file
    bool isFolder;           ///< True if this is a folder, false if it's a file
    Cluster *clusterList;    ///< List of clusters allocated to this file
};

/**
 * @struct Fat
 * @brief Represents the File Allocation Table (FAT) of the system.
 */
struct Fat {
    vector<BlockStatus> blockStatus;   ///< Status of each block (free or occupied)
    int totalBlocks;                   ///< Total number of blocks in the system
    int blockSize;                     ///< Size of each block
    vector<File *> files;              ///< List of files stored in the FAT system
};

/**
 * @class FATSYSTEM
 * @brief Implements a FAT (File Allocation Table) file system for managing files and folders on a disk.
 */
class FATSYSTEM {
    Fat *fat;               ///< Pointer to the FAT structure
    Disk *disk;             ///< Pointer to the Disk object

public:
    /**
     * @brief Constructor for the FATSYSTEM.
     * @param diskSpace Total space of the disk in blocks.
     * @param disk_ Pointer to the Disk object.
     */
    FATSYSTEM(int diskSpace, Disk *disk_);

    /**
     * @brief Destructor for the FATSYSTEM.
     */
    ~FATSYSTEM();

    /**
     * @brief Creates a cluster at the specified block index.
     * @param blockIndex Index of the block.
     * @return Pointer to the newly created Cluster.
     */
    Cluster *createCluster(int blockIndex);

    /**
     * @brief Gets the total free space available on the disk.
     * @return The free space in blocks.
     */
    int getFreeDiskSpace();

    /**
     * @brief Creates a new file or folder.
     * @param name_ Name of the file.
     * @param author_ Name of the file's author.
     * @param data Data to be written to the file.
     * @param parentName Name of the parent folder.
     * @param isFolder Flag to indicate whether it's a folder (true) or file (false).
     * @return Pointer to the newly created File.
     */
    File *createFile(char *name_, char *author_, string data, char *parentName, bool isFolder = false);

    /**
     * @brief Deletes a file or folder from the FAT system.
     * @param fileName Name of the file to delete.
     * @param deleteFolderInFolder Set true to delete folders recursively.
     * @param ignoreFolderTyp Set true to ignore folder type checking.
     */
    void deleteFile(const char *fileName, bool deleteFolderInFolder = false, bool ignoreFolderTyp = false);

    /**
     * @brief Displays the current FAT table and file status.
     */
    void showFat();

    /**
     * @brief Calculates the fragmentation percentage of the disk.
     * @return The fragmentation percentage.
     */
    float getFragmentation();

    /**
     * @brief Defragments the disk by rearranging clusters.
     */
    void defragDisk();

    /**
     * @brief Gets the current Disk object.
     * @return Pointer to the Disk object.
     */
    Disk *getDisk() const;

    /**
     * @brief Sets a new Disk object.
     * @param newDisk Pointer to the new Disk object.
     */
    void setDisk(Disk *newDisk);

    /**
     * @brief Gets the current FAT table.
     * @return Pointer to the FAT structure.
     */
    Fat *getFat() const;

    /**
     * @brief Sets a new FAT table.
     * @param newFat Pointer to the new FAT structure.
     */
    void setFat(Fat *newFat);

    /**
     * @brief Finds a file by its name.
     * @param fileName Name of the file to search for.
     * @return Pointer to the found File object, or nullptr if not found.
     */
    File *findFile(char *fileName);

    /**
     * @brief Edits the data of a file.
     * @param fileName Name of the file to edit.
     * @param data New data to be written to the file.
     * @return Number of blocks used after editing.
     */
    int editData(char *fileName, string data);

    /**
     * @brief Gets the number of blocks used by a file.
     * @param fileName Name of the file.
     * @return Number of blocks used.
     */
    int getNumberOfBlocks(char *fileName);

    /**
     * @brief Gets the last cluster in the chain for a file.
     * @param fileName Name of the file.
     * @return Pointer to the last Cluster in the file.
     */
    Cluster *getLastClusterOfFile(char *fileName);

    /**
     * @brief Displays the cluster chain of a file.
     * @param fileName Name of the file.
     */
    void showCluster(char *fileName);

    /**
     * @brief Retrieves the data stored in a file.
     * @param fileName Name of the file.
     * @return String containing the file data.
     */
    string getDataOfFile(char *fileName);

    /**
     * @brief Splits a string into smaller parts.
     * @param inputString The input string.
     * @return Vector of strings containing the parts.
     */
    vector<string> splitStringIntoParts(string inputString);

    /**
     * @brief Gets the list of files in a specified folder.
     * @param folderName Name of the folder.
     * @return List of File pointers in the folder.
     */
    QList<File *> getFilesInFolder(char *folderName);

    /**
     * @brief Finds a file by its name (const version).
     * @param fileName Name of the file to search for.
     * @return Pointer to the found File object, or nullptr if not found.
     */
    File *findFile(const char *fileName);

    /**
     * @brief Gets the list of folders inside a folder.
     * @param folderName Name of the folder.
     * @return List of Folder pointers inside the folder.
     */
    QList<File *> getFoldersInFolder(char *folderName);

    /**
     * @brief Renames a file inside a folder.
     * @param folder Name of the folder containing the file.
     * @param fileName Current name of the file.
     * @param newName New name of the file.
     */
    void renameFile(char *folder, char *fileName, char *newName);

    /**
     * @brief Finds the folder containing a file.
     * @param fileName Name of the file.
     * @return Name of the folder containing the file.
     */
    char *findFolderOfFile(char *fileName);

    /**
     * @brief Removes a file from a folder.
     * @param fileName Name of the file.
     * @param folderName Name of the folder.
     */
    void removeFileFromFolder(char *fileName, char *folderName);

    /**
     * @brief Adds a file to a folder.
     * @param parentName Name of the parent folder.
     * @param name_ Name of the file.
     * @return 0 if successful, non-zero otherwise.
     */
    int addFileToFolder(char *parentName, char *name_);

    /**
     * @brief Gets the folder containing a file.
     * @param fileName Name of the file.
     * @return Name of the folder containing the file.
     */
    char *getFolderOfFile(char *fileName);

    /**
     * @brief Deletes a folder and its contents.
     * @param fileName Name of the folder.
     */
    void deleteFolder(const char *fileName);

    /**
     * @brief Creates a unique name for a file by appending unique text.
     * @param fileName Original file name.
     * @param uniqueText Text to append for uniqueness (default is "-C").
     * @return The unique file name.
     */
    string createUniqueName(char *fileName, char *uniqueText = "-C");

    /**
     * @brief Checks if a file exists in the system.
     * @param fileName Name of the file to check.
     * @return True if the file exists, false otherwise.
     */
    bool existFile(char *fileName);

    /**
     * @brief Copies a file to another folder.
     * @param fileName Name of the file to copy.
     * @param folderName Name of the destination folder.
     */
    void copyFile(char *fileName, char *folderName);
};

#endif // FATSYSTEM_H

#ifndef CDSYSTEM_H
#define CDSYSTEM_H

#include "cdrom.h"
#include "fatSystem.h"
#include "inodesystem.h"
#include <string>

/**
 * @brief The CDSYSTEM class manages the operations for CD-ROMs using different file systems.
 *
 * This class provides functionality for handling ejected and inserted CDs, burning CDs,
 * processing files and folders, and managing file systems like FAT and INODE.
 */
class CDSYSTEM {
public:

    vector<CDROM *> ejectedCds;      ///< List of ejected CDs.
    vector<CDROM *> insertedCds;     ///< List of inserted CDs.
    INODESYSTEM *ISys;               ///< Pointer to the INODE file system.
    FATSYSTEM *FSys;                 ///< Pointer to the FAT file system.

    /**
     * @brief Constructor for CDSYSTEM.
     *
     * Initializes the system with pointers to INODESYSTEM and FATSYSTEM.
     *
     * @param Isys Pointer to the INODE system.
     * @param Fsys Pointer to the FAT system.
     */
    CDSYSTEM(INODESYSTEM *Isys, FATSYSTEM *Fsys);

    /**
     * @brief Get the list of ejected CDs.
     * @return Vector of ejected CDs.
     */
    vector<CDROM *> getEjectedCds() const;

    vector<INode *> nodes;           ///< List of INode objects.
    vector<File *> files;            ///< List of File objects.

    /**
     * @brief Set the list of ejected CDs.
     * @param newEjectedCds New vector of ejected CDs.
     */
    void setEjectedCds(const vector<CDROM *> &newEjectedCds);

    /**
     * @brief Add a new ejected CD based on the port name.
     * @param portName_ The port name of the CD to eject.
     */
    void addEjectedCd(string portName_);

    /**
     * @brief Get the list of inserted CDs.
     * @return Vector of inserted CDs.
     */
    vector<CDROM *> getInsertedCds() const;

    /**
     * @brief Set the list of inserted CDs.
     * @param newInsertedCds New vector of inserted CDs.
     */
    void setInsertedCds(const vector<CDROM *> &newInsertedCds);

    /**
     * @brief Create a file entry based on the INode.
     * @param node Pointer to the INode.
     * @return Pointer to the created FileEntry.
     */
    FileEntry *createFileEntry(INode *node);

    /**
     * @brief Process files to create base entries on a CD-ROM.
     *
     * Processes the files and nodes and adds them to the CDROM's base blocks.
     *
     * @param nodes_ List of INode objects.
     * @param files_ List of File objects.
     * @param cd Pointer to the CDROM.
     * @param leastStartBlock The starting block for the files.
     * @return Vector of base entries created from the files.
     */
    vector<Base *> processFiles(vector<INode *> nodes_, vector<File *> files_, CDROM *cd, int leastStartBlock);

    /**
     * @brief Process a folder to create base entries on a CD-ROM.
     *
     * Processes the INode object representing the folder.
     *
     * @param node_ Pointer to the INode representing the folder.
     * @param cd Pointer to the CDROM.
     * @param leastStartBlock The starting block for the folder.
     * @return Vector of base entries created from the folder.
     */
    vector<Base *> processFolder(INode *node_, CDROM *cd, int leastStartBlock);

    /**
     * @brief Burn a CD-ROM with the current system state.
     * @return Pointer to the burned CDROM.
     */
    CDROM *burnCD();

    /**
     * @brief Get the number of files in a folder.
     *
     * Counts the number of files in a folder represented by a File object.
     *
     * @param file_ Pointer to the File object.
     * @return The number of files in the folder.
     */
    int getNumberOfFilesInFolder(File *file_);

    /**
     * @brief Get the number of files in a folder.
     *
     * Counts the number of files in a folder represented by an INode object.
     *
     * @param node_ Pointer to the INode object.
     * @return The number of files in the folder.
     */
    int getNumberOfFilesInFolder(INode *node_);

    /**
     * @brief Get the total number of files across multiple nodes and files.
     * @param nodes_ List of INode objects.
     * @param files_ List of File objects.
     * @return The total number of files.
     */
    int getNumberOfFiles(vector<INode *> nodes_, vector<File *> files_);

    /**
     * @brief Create data blocks on a CD-ROM.
     * @param cd Pointer to the CDROM.
     * @param data The data to store in the blocks.
     * @param leastStartBlock The starting block for the data.
     * @return The number of blocks created.
     */
    int createDataBlocks(CDROM *cd, string data, int leastStartBlock);

    /**
     * @brief Create a file entry based on a File object.
     * @param file Pointer to the File object.
     * @return Pointer to the created FileEntry.
     */
    FileEntry *createFileEntry(File *file);

    /**
     * @brief Process a folder to create base entries on a CD-ROM.
     *
     * Processes the folder represented by a File object.
     *
     * @param file_ Pointer to the File object.
     * @param cd Pointer to the CDROM.
     * @param leastStartBlock The starting block for the folder.
     * @return Vector of base entries created from the folder.
     */
    vector<Base *> processFolder(File *file_, CDROM *cd, int leastStartBlock);

    /**
     * @brief Process a folder represented by an INode object.
     *
     * Processes the folder represented by the INode object and creates base entries.
     *
     * @param node_ Pointer to the INode object.
     * @param cd Pointer to the CDROM.
     * @param leastStartBlock The starting block for the folder.
     * @return Vector of base entries created from the folder.
     */
    vector<Base *> processFolder2(INode *node_, CDROM *cd, int leastStartBlock);

    /**
     * @brief Read the contents of a folder from the CD-ROM.
     *
     * Reads the folder's contents and processes it based on the current file system.
     *
     * @param cd Pointer to the CDROM.
     * @param currentSystem The file system being used (FAT or INODE).
     * @param folder Pointer to the FileEntry representing the folder.
     * @param folderName The name of the folder.
     */
    void readFolder(CDROM *cd, int currentSystem, FileEntry *folder, string folderName);

    /**
     * @brief Find a specific file entry on the CD-ROM.
     * @param cd Pointer to the CDROM.
     * @param baseBlock The base block where the file entry is located.
     * @return Pointer to the found FileEntry.
     */
    FileEntry *findEntry(CDROM *cd, int baseBlock);

public slots:
            /**
             * @brief Slot to handle reading the contents of a CD-ROM.
             * @param cd Pointer to the CDROM.
             * @param currentSystem The current file system being used (FAT or INODE).
             */
            void readCD(CDROM *cd, int currentSystem);
};

#endif // CDSYSTEM_H

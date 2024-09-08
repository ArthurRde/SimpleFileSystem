#ifndef DIALOGEJECT_H
#define DIALOGEJECT_H

#include "cdsystem.h"
#include <QDialog>
#include <string>

using namespace std;

namespace Ui {
    class DialogEject;
}

/**
 * @brief The DialogEject class represents a dialog window for ejecting CDs from the system.
 *
 * This class allows users to select a CD and eject it using the graphical interface.
 */
class DialogEject : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs a DialogEject object.
     *
     * Initializes the dialog for ejecting CDs, setting up the UI and system references.
     *
     * @param sys_ Pointer to the CDSYSTEM object, representing the CD system.
     * @param parent Pointer to the parent widget (defaults to nullptr).
     */
    explicit DialogEject(CDSYSTEM *sys_, QWidget *parent = nullptr);

    /**
     * @brief Destructor for DialogEject.
     *
     * Cleans up the allocated resources, including the UI object.
     */
    ~DialogEject();

    CDSYSTEM *sys;  ///< Pointer to the CDSYSTEM object representing the CD system.

public slots:
            /**
             * @brief Adds a CD name to the combo box.
             *
             * This slot is used to dynamically add CD names to the combo box for selection.
             *
             * @param name The name of the CD to be added to the combo box.
             */
            void addCdToComboBox(string name);

    /**
     * @brief Removes a CD name from the combo box.
     *
     * This slot is used to remove a specific CD from the combo box based on its name.
     *
     * @param name The name of the CD to be removed from the combo box.
     */
    void removeCdFromComboBox(string name);

    /**
     * @brief Clears all CDs from the combo box.
     *
     * This slot is used to remove all entries from the CD combo box.
     */
    void clearCdFromComboBox();

    /**
     * @brief Ejects the selected CD.
     *
     * This slot is triggered to eject the currently selected CD from the system.
     */
    void eject();

private slots:
            /**
             * @brief Handles the event when the eject button is clicked.
             *
             * This slot is triggered when the user clicks the eject button in the dialog.
             */
            void on_pushButton_clicked();

private:
    Ui::DialogEject *ui;  ///< Pointer to the UI object for this dialog.
};

#endif // DIALOGEJECT_H

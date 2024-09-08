#ifndef DIALOGRENAME_H
#define DIALOGRENAME_H

#include <QDialog>
#include "string"

using namespace std;

namespace Ui {
    class DialogRename;
}

/**
 * @class DialogRename
 * @brief A dialog for renaming items.
 */
class DialogRename : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for DialogRename.
     * @param name The current name of the item.
     * @param parent Pointer to the parent widget.
     */
    explicit DialogRename(string name, QWidget *parent = nullptr);

    /**
     * @brief Destructor for DialogRename.
     */
    ~DialogRename();

    /**
     * @brief Gets the new name entered by the user.
     * @return The new name as a string.
     */
    string getName();

private:
    /**
     * @brief Pointer to the UI.
     */
    Ui::DialogRename *ui;
};

#endif // DIALOGRENAME_H
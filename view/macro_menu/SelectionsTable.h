#ifndef SELECTIONSTABLE_H
#define SELECTIONSTABLE_H


#include "model/MacroMetadata.h"
#include <QTableWidget>
#include <QMouseEvent>


/**
 * @brief The SelectionsTable class
 * Selections table UI class. Displays all of the possible Macros that can be selected.
 */
class SelectionsTable : public QTableWidget
{
    Q_OBJECT

public:

    explicit SelectionsTable(QWidget *parent);


    /**
     * @brief getRowAt
     * Gets the table row contents at a specified row index.
     * @param row
     * The zero grounded index of the row to be fetched.
     * @return
     * A tuple containing the contents of the row.
     */
    MacroMetadata getRowAt(int row) const;


    /**
     * @brief getNumSelectedRows
     * Gets the number of selected rows within the selectionsTable UI element.
     * @return
     * The number of selected rows.
     */
    int getNumSelectedRows() const;


    /**
     * @brief getSelectedRows
     * Gets the selected rows within the selections table.
     * @return
     * A list of the selected row contents.
     */
    QList<MacroMetadata> getSelectedRows() const;

    /**
     * @brief getSelectedRows
     * Gets the selected rows' IDs within the selections table.
     * @return
     * A list of the selected row's IDs.
     */
    QList<int> getSelectedRowIds() const;


    /**
     * @brief getSortOrder
     * Gets the sort order for the table.
     * @return
     * The current sort order.
     */
    MacroMetadataSortOrder getSortOrder() const;

    /**
     * @brief setSortOrder
     * Sets the sort order for the Macros Selection Table.
     * @param sortOrder
     * The sort order to be set.
     */
    void setSortOrder(MacroMetadataSortOrder sortOrder);


    /**
     * @brief refresh
     * Refreshes the table with a new set of rows. Clears out any old rows.
     * @param rows
     * A list of the new rows.
     */
    void refresh(const QList<MacroMetadata> &rows);


    /**
     * @brief insertRow
     * Inserts a new row in the table.
     * @param row
     * The new row.
     */
    void insertRow(const MacroMetadata &row);

    /**
     * @brief insertRows
     * Inserts new rows in the table.
     * @param rows
     * A list of the new rows.
     */
    void insertRows(const QList<MacroMetadata> &rows);


    /**
     * @brief removeSelectedRows
     * Removes all selected rows.
     */
    void removeSelectedRows();


signals:

    /**
     * @brief leftDoubleClick
     * Emitted when a left double click occurs.
     */
    void leftDoubleClick();

    /**
     * @brief scrollBarHitBottom
     * Emitted whenever the vertical scrollbar slider hits the bottom of the scrollbar track.
     */
    void scrollBarHitBottom();


private:

    /**
     * @brief mouseDoubleClickEvent
     * Monitors mouse double click events.
     * @param e The mouse event.
     */
    void mouseDoubleClickEvent(QMouseEvent *e);


private slots:

    /**
     * @brief handleVertScrollBarMovement
     * Handles a vertical scrollbar movement event.
     * @param trackPos
     * The position of the scrollbar slider in the scrollbar track.
     */
    void handleVertScrollBarMovement(int trackPos);
};


#endif // SELECTIONSTABLE_H

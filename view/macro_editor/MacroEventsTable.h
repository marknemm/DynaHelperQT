#ifndef MACROEVENTSTABLE_H
#define MACROEVENTSTABLE_H


#include <QWidget>
#include <QTableWidget>
#include <QList>
#include <QImage>
#include <QDropEvent>
#include "model/MacroEvent.h"
#include "controller/macro_editor/MacroEditorEventListener.h"


/**
 * @brief The MacroEventsTable class
 * The Macro Events Table inside of the Macro Editor view.
 */
class MacroEventsTable : public QTableWidget
{
    Q_OBJECT

public:

    explicit MacroEventsTable(QWidget *parent = nullptr);

    void setEventListener(MacroEditorEventListener &eventListener);

    /**
     * @brief addMacroEvents
     * Populates the table with the given list of Macro Events.
     * @param macroEvents
     * The list of Macro Events.
     */
    void addMacroEvents(const QList<MacroEvent> &macroEvents);


    /**
     * @brief refreshColumnWidths
     * Refreshes the column widths for the table.
     * @param MacroEvents
     * A list of Macro Events that can be used by the UI for sizing display purposes.
     */
    void refreshColumnWidths(const QList<MacroEvent> &macroEvents);


    /**
     * @brief getNumSelectedRows
     * Gets the number of selected rows.
     * @return
     * The number of selected rows.
     */
    int getNumSelectedRows() const;

    /**
     * @brief getSelectedEventInds
     * Gets the selected events' indexes. These also correspond to the Macro Events table's row numbers.
     * @return
     * A list of the event indexes of selected events/rows.
     */
    QList<int> getSelectedEventInds() const;

    /**
     * @brief isSelectedRowDisabled
     * Checks if the selected row is disabled.
     * @return
     * True if the selected row is disabled, false if not.
     */
    bool isSelectedRowDisabled() const;


private slots:

    /**
     * @brief handleCellChanged
     * Handles a cell change by notifying any Macro Editor Event Listeners.
     * @param row
     * The row of the cell that was changed.
     * @param column
     * The column of the cell that was changed.
     */
    void handleCellChanged(int row, int column);


private:

    /**
     * @brief The TableCellTypes enum
     * The possible types for table cell values.
     */
    enum TableCellTypes {
        INT = 0,
        STRING = 1,
        ICON = 2,
        CHECKBOX = 3
    };


    // Macro Events Columns
    const static int EVENT_INDEX_COL;
    const static int EVENT_DESCRIPTION_COL;
    const static int EVENT_KEY_STRING_COL;
    const static int EVENT_DELAY_COL;
    const static int EVENT_DURATION_COL;
    const static int EVENT_AUTO_CORRECT_COL;
    const static int EVENT_SCREENSHOT_COL;

    MacroEditorEventListener *_editorEventListener;

    /**
     * @brief insertDummyRowCells
     * Inserts empty disabled row to fill in gaps for non-uniform events.
     * @param eventInd
     * The event index of the non-uniform event.
     */
    void insertDummyRowCells(int eventInd);


    /**
     * @brief insertIndexCellAtRow
     * Inserts an index cell with a given value at a given row.
     * @param r
     * The row to insert an index cell at.
     * @param index
     * The index to set the index cell value to.
     */
    void insertIndexCellAtRow(int r, int index);

    /**
     * @brief insertDescriptionCellAtRow
     * Inserts an event description for a given event at a given row.
     * @param r
     * The row to insert the description for the event at.
     * @param event
     * The event that the description will be formed for.
     */
    void insertDescriptionCellAtRow(int r, const MacroEvent &event);

    /**
     * @brief insertKeyStringCellAtRow
     * Inserts the key string cell with a given value at a given row.
     * @param r
     * The row to insert a key string cell at.
     * @param event
     * The event to get the key string from.
     */
    void insertKeyStringCellAtRow(int r, const MacroEvent &event);

    /**
     * @brief insertDelayCellAtRow
     * Inserts a delay cell with a given value at a given row.
     * @param r
     * The row to insert the delay cell at.
     * @param delay
     * The delay to set the delay cell value to.
     */
    void insertDelayCellAtRow(int r, int delay);

    /**
     * @brief insertDurationCellAtRow
     * Inserts a duration cell with a given value at a given row.
     * @param r
     * The row to insert the duration cell at.
     * @param duration
     * The duration to set the duration cell value to.
     */
    void insertDurationCellAtRow(int r, int duration);

    /**
     * @brief insertAutoCorrectCellAtRow
     * Inserts an auto correct checkbox cell with a given value at a given row.
     * @param r
     * The row to insert the auto correct cell at.
     * @param autoCorrect
     * True to set the auto correct checkbox checked, false to set unchecked.
     * @param eventType
     * The type of the event that this row is associated with.
     */
    void insertAutoCorrectCellAtRow(int r, bool autoCorrect, MacroEventType eventType);

    /**
     * @brief insertScreenshotCellAtRow
     * Inserts a screenshot cell with a given value at a given row.
     * @param r
     * The row to insert the screenshot cell at.
     * @param screenshot
     * The screenshot to set the screenshot cell value to.
     * @param screenshotOrg
     * The X,Y origin of the screenshot image in screen coordinates.
     * @param mouseLoc
     * The location of the mouse event associated witht he screenshot in screen coordinates.
     */
    void insertScreenshotCellAtRow(int r, const QImage &screenshot, const QPoint &screenshotOrg, const QPoint &mouseLoc);


    /**
     * @brief genMouseEventDescr
     * Generates the description for a given Macro Mouse Event.
     * @param mEvent
     * The Macro Mouse Event to generate the description for.
     * @return
     * The generated description.
     */
    QString genMouseEventDescr(const MacroMouseEvent &mEvent) const;

    /**
     * @brief genKeyboardEventDescr
     * Generates a description for a given Macro Keyboard Event.
     * @param kEvent
     * The keyboard event to generate the description for.
     */
    QString genKeyboardEventDescr(const MacroKeyboardEvent &kEvent) const;

    /**
     * @brief The TableWidgetRow class
     * Container for a row of TableWidgetItem objects.
     */
    class TableWidgetRow
    {

    public:

        explicit TableWidgetRow(int row);

        /**
         * @brief Comparator
         * Comparator for two TableWidgetRow objects.
         */
        typedef struct LTComparator
        {
            bool operator()(const TableWidgetRow &left, const TableWidgetRow &right) const;
        } LTComparator;

        /**
         * @brief append
         * Appends a QTableWidgetItem to the row.
         * @param item
         * The item to append.
         */
        void append(QTableWidgetItem* item);

        /**
         * @brief append
         * Appends a list of QTableWidgetItems to the row.
         * @param items
         * The list of items to append.
         */
        void append(const QList<QTableWidgetItem*> &items);

        /**
         * @brief getItemAtCol
         * Gets the QTableWidgetItem belonging to this row at a specified column.
         * @param col
         * The column to get from.
         */
        QTableWidgetItem* getItemAtCol(int col) const;

        /**
         * @brief getRowInd
         * Gets the row index.
         * @return
         * The row index.
         */
        int getRowInd() const;

        /**
         * @brief columnCount
         * Gets the column count, or rather, the number of items in the row.
         * @return
         * The column count.
         */
        int columnCount() const;

    private:

        /**
         * @brief _row
         * The row number.
         */
        int _row;

        /**
         * @brief _rowItems
         * The QTableWidgetItems belonging to the row.
         */
        QList<QTableWidgetItem*> _rowItems;
    };

    /**
     * @brief eventFilter
     * General event filter for main menu UI.
     * @param obj
     * The UI element that would normally receive the event.
     * @param event
     * The event that is to be filtered.
     * @return
     * true if should stop further handling of event, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

    /**
     * @brief dropEvent
     * Handles the drop event for table rows.
     * @param event
     * The drop event information.
     */
    void dropEvent(QDropEvent *event) override;
};


#endif // MACROEVENTSTABLE_H

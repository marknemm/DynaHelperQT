#include "MacroEventsTable.h"
#include "record_img/RecordImageUtil.h"
#include "QTableWidgetNumberDelegate.h"
#include <QCursor>
#include <QHeaderView>
#include <QBrush>
#include <QKeyEvent>
#include <QCheckBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QModelIndexList>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QItemSelectionModel>
#include <QDebug>


const int MacroEventsTable::EVENT_INDEX_COL         = 0;
const int MacroEventsTable::EVENT_DESCRIPTION_COL   = 1;
const int MacroEventsTable::EVENT_KEY_STRING_COL    = 2;
const int MacroEventsTable::EVENT_DELAY_COL         = 3;
const int MacroEventsTable::EVENT_DURATION_COL      = 4;
const int MacroEventsTable::EVENT_AUTO_CORRECT_COL  = 5;
const int MacroEventsTable::EVENT_SCREENSHOT_COL    = 6;


void MacroEventsTable::setEventListener(MacroEditorEventListener &eventListener)
{
    _editorEventListener = &eventListener;
    installEventFilter(this);
}


MacroEventsTable::MacroEventsTable(QWidget *parent)
    : QTableWidget(parent)
{
    int rowHeight = 50;
    horizontalHeader()->setFont(QFont("Arial", 14));
    // Sizing.
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(rowHeight);
    setIconSize(QSize(300, rowHeight));
    // Number Validator.
    QList<int> numberColIndexes;
    numberColIndexes.append(EVENT_DELAY_COL);
    numberColIndexes.append(EVENT_DURATION_COL);
    setItemDelegate(new QTableWidgetNumberDelegate(numberColIndexes));
}


void MacroEventsTable::addMacroEvents(const QList<MacroEvent> &macroEvents)
{
    // Stop update event listener (only want user updates)!
    disconnect(this, SIGNAL(cellChanged(int,int)), this, SLOT(handleCellChanged(int, int)));

    // Stop inefficinet widget model signals until after bulk add.
    model()->blockSignals(true);

    // Insert data for each event.
    foreach (MacroEvent event, macroEvents) {
        // Only add row if needed for the insertion of our uniform Macro Event.
        if (event.index >= rowCount()) insertRow(event.index);

        // Make sure we fill in empty disabled rows for non-uniform Macro Events.
        if (event.type == DummyEvent) {
            insertDummyRowCells(event.index);
        }
        // Else this will be a uniform event row and will be filled in like normal.
        else {
            // Insert all of the cells for this uniform event row.
            insertIndexCellAtRow(event.index, event.index);
            insertDescriptionCellAtRow(event.index, event);
            insertKeyStringCellAtRow(event.index, event);
            insertDelayCellAtRow(event.index, event.delayMs);
            insertDurationCellAtRow(event.index, event.durationMs);
            insertAutoCorrectCellAtRow(event.index, event.mouseEvent.autoCorrect, event.type);
            // Insert the screenshot cell when we have better size information in refreshColumnWidths() below!
        }
    }

    // Notify widget model that it can send signals now that we are done with bulk insert.
    model()->blockSignals(false);
    model()->layoutChanged();

    // Reinstate update event listener to listen for user events.
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(handleCellChanged(int, int)));
}


bool MacroEventsTable::isSelectedRowDisabled() const
{
    // Only disabled rows will have an empty description entry.
    if (getNumSelectedRows() == 1) {
        int row = getSelectedEventInds().first();
        return item(row, EVENT_DESCRIPTION_COL)->text().isEmpty();
    }

    return false;
}


void MacroEventsTable::refreshColumnWidths(const QList<MacroEvent> &macroEvents)
{
    horizontalHeader()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
    setIconSize(QSize(columnWidth(columnCount() - 1), 50));

    // Stop update event listener (only want user updates)!
    disconnect(this, SIGNAL(cellChanged(int,int)), this, SLOT(handleCellChanged(int, int)));

    // Update the screenshot images present in the table.
    foreach (const MacroEvent &event, macroEvents) {
        if (event.type == MouseEvent) {
            insertScreenshotCellAtRow(event.index, event.mouseEvent.screenshotImg,
                                      event.mouseEvent.screenshotRect.topLeft(), event.mouseEvent.loc);
        }
    }

    // Reinstate update event listener to listen for user events.
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(handleCellChanged(int, int)));
}


int MacroEventsTable::getNumSelectedRows() const
{
    return selectionModel()->selectedRows().size();
}


QList<int> MacroEventsTable::getSelectedEventInds() const
{
    int eventInd;
    QList<int> selectedEventInds;
    QModelIndexList selectedRows = selectionModel()->selectedRows();
    // Iterate over each selected row and extract the event indexes.
    foreach (const QModelIndex &selectedRow, selectedRows) {
        eventInd = item(selectedRow.row(), 0)->text().toInt();
        selectedEventInds.append(eventInd);
    }
    return selectedEventInds;
}


void MacroEventsTable::handleCellChanged(int row, int column)
{
    QTableWidgetItem *item = this->item(row, column);
    QString updateStr = item->text();
    bool checked = (item->checkState() == Qt::CheckState::Checked);

    qDebug() << "Cell changed at: (" << row << ", " << column << ")";
    if (column == EVENT_DELAY_COL || column == EVENT_DURATION_COL) {
        qDebug() << "New Cell value is: " << updateStr;
    }
    else {
        qDebug() << "Checkbox state is: " << checked;
    }

    // Feed all update events to the event listener (controller) for processing in model if we have an event listener set.
    if (_editorEventListener != nullptr) {
        switch (column) {
        case EVENT_KEY_STRING_COL:      _editorEventListener->updateEventKeyString(row, updateStr);         break;
        case EVENT_DELAY_COL:           _editorEventListener->updateEventDelay(row, updateStr.toInt());     break;
        case EVENT_DURATION_COL:        _editorEventListener->updateEventDuration(row, updateStr.toInt());  break;
        case EVENT_AUTO_CORRECT_COL:    _editorEventListener->updateEventAutoCorrect(row, checked);         break;
        }
    }
}


void MacroEventsTable::insertDummyRowCells(int eventInd)
{
    QTableWidgetItem *item;
    for (int c = 0; c < columnCount(); c++) {
        item = new QTableWidgetItem();
        if (c == EVENT_INDEX_COL) {
            item->setText(QString::number(eventInd));
        }
        item->setFlags(item->flags() ^ Qt::ItemIsEditable); // Disabled
        item->setBackgroundColor(QColor(250, 250, 250));
        setItem(eventInd, c, item);
    }
}


void MacroEventsTable::insertIndexCellAtRow(int row, int index)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(index), INT);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable); // Uneditable
    setItem(row, EVENT_INDEX_COL, item);
}


void MacroEventsTable::insertDescriptionCellAtRow(int row, const MacroEvent &event)
{
    QString description;
    // Form event description.
    switch (event.type) {
    case MouseEvent:    description = genMouseEventDescr(event.mouseEvent);         break;
    case KeyboardEvent: description = genKeyboardEventDescr(event.keyboardEvent);   break;
    }
    // Generate description cell.
    QTableWidgetItem *item = new QTableWidgetItem(description, STRING);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable); // Uneditable
    setItem(row, EVENT_DESCRIPTION_COL, item);
}


void MacroEventsTable::insertKeyStringCellAtRow(int r, const MacroEvent &event)
{
    QTableWidgetItem *item;
    bool isKeyStringEvent =    event.type == KeyboardEvent
                            && event.keyboardEvent.type == KeyString;

    // Only take event key string if the vent type is Keyboard Event!
    if (isKeyStringEvent) {
        QString keyString = event.keyboardEvent.keyString;
        item = new QTableWidgetItem(keyString, STRING);
    }
    // Else, make sure the cell is uneditable.
    else {
        item = new QTableWidgetItem("", STRING);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable); // Uneditable
    }

    setItem(r, EVENT_KEY_STRING_COL, item);
}


void MacroEventsTable::insertDelayCellAtRow(int row, int delay)
{
    QString delayTxt = delay >= 0 ? QString::number(delay)
                                  : "";
    QTableWidgetItem *item = new QTableWidgetItem(delayTxt, INT);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, EVENT_DELAY_COL, item);
}


void MacroEventsTable::insertDurationCellAtRow(int row, int duration)
{
    QString durationTxt = duration >= 0 ? QString::number(duration)
                                        : "";
    QTableWidgetItem *item = new QTableWidgetItem(durationTxt, ICON);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, EVENT_DURATION_COL, item);
}


void MacroEventsTable::insertAutoCorrectCellAtRow(int row, bool autoCorrect, MacroEventType eventType)
{
    /*QWidget *checkWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();

    QHBoxLayout *layout = new QHBoxLayout(checkWidget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    checkWidget->setLayout(layout);

    // Set checked based off of auto correct state.
    checkBox->setChecked(autoCorrect);
    // Disable checkbox if not a mouse event.
    checkBox->setDisabled(eventType != MacroEventType::MouseEvent);*/

    QTableWidgetItem *item = new QTableWidgetItem(CHECKBOX);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable); // Uneditable
    item->setToolTip("Check to enable auto correct");
    //setCellWidget(row, 4, checkWidget);
    if (eventType == MacroEventType::MouseEvent) {
        item->setCheckState(autoCorrect ? Qt::Checked : Qt::Unchecked);
    }
    setItem(row, EVENT_AUTO_CORRECT_COL, item);
}


void MacroEventsTable::insertScreenshotCellAtRow(int row, const QImage &screenshot, const QPoint &screenshotOrg, const QPoint &mouseLoc)
{
    QTableWidgetItem *item;

    // We have an image to display
    if (screenshot.size().width() != 0) {
        QImage cropImg;

        // Blow up extremely small images.
        if (screenshot.width() < 30 && screenshot.height() < 30) {
            cropImg = screenshot.scaled(screenshot.width() >= 30 ? screenshot.width() : 30,
                                        screenshot.height() >= 30 ? screenshot.height() : 30);
        }
        else {
            cropImg = screenshot;
        }

        // Crop the screenshot to show in table cell.
        QRect cropRect(QPoint(-1, -1), iconSize());
        Qt::GlobalColor fillRemainingClr = Qt::white;
        cropImg = RecordImageUtil::cropImg(cropImg,
                                           cropRect,
                                           &fillRemainingClr,
                                           &mouseLoc,
                                           &screenshotOrg);

        QIcon icon(QPixmap::fromImage(cropImg));
        item = new QTableWidgetItem(icon, "", ICON);
        // TODO: Clip screenshot display to show local region where mouse event happened!
        int dim = (screenshot.width() < 300) ? screenshot.width()
                                             : 300;
        item->setSizeHint(QSize(dim, dim));
    }
    // We do not have image to display (still need to create uneditable cell).
    else {
        item = new QTableWidgetItem(ICON);
    }

    // Make sure the screenshot cell is not editable regardless of the presence of an image.
    item->setFlags(item->flags() ^ Qt::ItemIsEditable); // Uneditable
    setItem(row, EVENT_SCREENSHOT_COL, item);
}


QString MacroEventsTable::genMouseEventDescr(const MacroMouseEvent &mEvent) const
{
    QString description = getMacroMouseEventTypeStr(mEvent.type);
    bool locationSensitive =    mEvent.type != MacroMouseEventType::ScrollDown
                             && mEvent.type != MacroMouseEventType::ScrollUp;

    // Mouse Location Event Description.
    if (locationSensitive) {
        description += ": (" + QString::number(mEvent.loc.x()) + ", " + QString::number(mEvent.loc.y()) + ")";
    }

    return description;
}


QString MacroEventsTable::genKeyboardEventDescr(const MacroKeyboardEvent &kEvent) const
{
    QString description = getMacroKeyboardEventTypeStr(kEvent.type);

    if (kEvent.type != MacroKeyboardEventType::KeyString) {
        description += ": ";
        if (kEvent.mod1 != -1) {
            description += getKeyCodeStr(kEvent.mod1) + " ";
        }
        if (kEvent.mod2 != -1) {
            description += getKeyCodeStr(kEvent.mod2) + " ";
        }

        description += getKeyCodeStr(kEvent.keyCode, kEvent.mod1, kEvent.mod2, kEvent.capsLock, kEvent.numLock);
    }

    return description;
}


MacroEventsTable::TableWidgetRow::TableWidgetRow(int row)
    : _row(row),
      _rowItems()
{}


bool MacroEventsTable::TableWidgetRow::LTComparator::operator() (const TableWidgetRow &left, const TableWidgetRow &right) const
{
    return (left.getRowInd() < right.getRowInd());
}


void MacroEventsTable::TableWidgetRow::append(QTableWidgetItem *item)
{
    _rowItems.append(item);
}


void MacroEventsTable::TableWidgetRow::append(const QList<QTableWidgetItem *> &items)
{
    _rowItems.append(items);
}


QTableWidgetItem* MacroEventsTable::TableWidgetRow::getItemAtCol(int col) const
{
    return _rowItems.at(col);
}


int MacroEventsTable::TableWidgetRow::getRowInd() const
{
    return _row;
}


int MacroEventsTable::TableWidgetRow::columnCount() const
{
    return _rowItems.size();
}


bool MacroEventsTable::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && obj == this) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();
        int mods = keyEvent->modifiers();
        int numSelectedRows = getNumSelectedRows();

        if (_editorEventListener != nullptr) {
            if (key == Qt::Key_Delete && numSelectedRows > 0) {
                _editorEventListener->deleteEvents(getSelectedEventInds());
            }
            else if (key == Qt::Key_C && mods == Qt::CTRL && numSelectedRows == 1) {
                _editorEventListener->copyEvents(getSelectedEventInds());
            }
            else if (key == Qt::Key_S && mods == Qt::CTRL) {
                _editorEventListener->saveEvents();
            }
        }
    }

    return false;
}


void MacroEventsTable::dropEvent(QDropEvent *event)
{
    // Stop inefficient model updates in bulk change.
    model()->blockSignals(true);

    if (event->source() == this) {
        QList<TableWidgetRow> selectedRows;

        // Get insert position for rows we are moving.
        bool dropAfterDestRow = false;
        int destRowBeforeDelete = indexAt(event->pos()).row();
        int destRow = destRowBeforeDelete;

        // Extract the selected rows and sort them by row index.
        QModelIndexList selectedModelIndexRows = selectionModel()->selectedRows();
        foreach (const QModelIndex &selectedModelIndexRow, selectedModelIndexRows) {
            int row = selectedModelIndexRow.row();
            // If we are going to remove rows before the destination row, then we must move destRow back!
            if (row < destRowBeforeDelete) {
                dropAfterDestRow = true;
                destRow--;
            }
            else if (row == destRowBeforeDelete) {
                // We must place the extracted items back in the table before stopping!
                foreach (TableWidgetRow selectedRow, selectedRows) {
                    for (int column = 0; column < selectedRow.columnCount(); column++) {
                        setItem(selectedRow.getRowInd(), column, selectedRow.getItemAtCol(column));
                    }
                }
                return; // Stop now, we are just dropping where we started!
            }
            selectedRows.append(TableWidgetRow(row));
            for (int column = 0; column < columnCount(); column++) {
                selectedRows.last().append(takeItem(row, column));
            }
        }
        std::sort(selectedRows.begin(), selectedRows.end(), TableWidgetRow::LTComparator());

        // Delete the selected row(s). Ok to delete now since we extracted out the QTableWidgetItems!
        // Important to do this in descending order so indexing works correctly!
        for (int i = selectedRows.size() - 1; i >= 0; i--) {
            removeRow(selectedRows.at(i).getRowInd());
        }

        // We are dropping outside the range of rows, so append to end of rows.
        if (destRowBeforeDelete < 0) { destRow = rowCount(); }
        // Else if we removed any rows before the destination row, we must place it after it!
        else if (dropAfterDestRow)   { destRow++;            }

        // Insert a row for each of the currently selected rows we are dropping.
        foreach (const TableWidgetRow &selectedRow, selectedRows) {
            insertRow(destRow);
            for (int column = 0; column < selectedRow.columnCount(); column++) {
                setItem(destRow, column, selectedRow.getItemAtCol(column));
            }
            destRow++;
        }
    }

    // Notify widget model that it can send signals now that we are done with bulk insert.
    model()->blockSignals(false);
    model()->layoutChanged();
    event->accept();
}

#include "SelectionsTable.h"
#include <exception>
#include <QScrollBar>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QHeaderView>
#include <QDebug>


SelectionsTable::SelectionsTable(QWidget *parent=nullptr) :
    QTableWidget(parent)
{
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(handleVertScrollBarMovement(int)));
}


MacroMetadata SelectionsTable::getRowAt(int row) const
{
    MacroMetadata macroMeta;
    macroMeta.id = item(row, 0)->text().toInt();
    macroMeta.name = item(row, 1)->text();
    return macroMeta;
}


int SelectionsTable::getNumSelectedRows() const
{
    return selectedItems().size() / columnCount(); // Rows = Items / Columns.
}


QList<MacroMetadata> SelectionsTable::getSelectedRows() const
{
    QList<MacroMetadata> selectedRows;
    QList<QTableWidgetItem*> selectedItems = this->selectedItems();
    typedef QList<QTableWidgetItem*>::iterator Iter;

    // Iterate every 2 items.
    for (Iter it = selectedItems.begin(); it != selectedItems.end(); it++) {
        MacroMetadata macroMeta;
        macroMeta.id = (*it)->text().toInt();
        macroMeta.name = (*(++it))->text();
        selectedRows.append(macroMeta);
    }

    return selectedRows;
}


QList<int> SelectionsTable::getSelectedRowIds() const
{
    QList<int> selectedRowIds;
    QList<QTableWidgetItem*> selectedItems = this->selectedItems();
    typedef QList<QTableWidgetItem*>::iterator Iter;

    // Iterate every 2 items.
    for (Iter it = selectedItems.begin(); it != selectedItems.end(); it++) {
        int id = (*(it++))->text().toInt();
        selectedRowIds.append(id);
    }

    return selectedRowIds;
}


MacroMetadataSortOrder SelectionsTable::getSortOrder() const
{
    int sortColInd = horizontalHeader()->sortIndicatorSection();
    Qt::SortOrder sortOrder = horizontalHeader()->sortIndicatorOrder();
    if (sortOrder == Qt::AscendingOrder) {
        if (sortColInd == 0) {
            return ID_ASC;
        }
        else {
            return NAME_ASC;
        }
    }
    if (sortColInd == 0) {
        return ID_DESC;
    }
    return NAME_DESC;
}


void SelectionsTable::setSortOrder(MacroMetadataSortOrder sortOrder)
{
    switch (sortOrder) {
    case ID_ASC:    sortItems(0);                                  break;
    case ID_DESC:   sortItems(0, Qt::SortOrder::DescendingOrder);  break;
    case NAME_ASC:  sortItems(1);                                  break;
    case NAME_DESC: sortItems(1, Qt::SortOrder::DescendingOrder);  break;
    default:
        throw std::runtime_error(("Error: incorrect sort order supplied to setSortOrder with value: " + std::to_string(sortOrder)).c_str());
    }
}


void SelectionsTable::refresh(const QList<MacroMetadata> &rows)
{
    model()->blockSignals(true);

    // Decrease the number of rows in the table if we don't have data to fill all of them.
    if (rows.size() < rowCount()) {
        setRowCount(rows.size());
    }

    // Set the contents of rows that already exist.
    int r = 0;
    foreach (MacroMetadata row, rows) {
        if (r == rowCount()) {
            break;
        }
        item(r, 0)->setData(Qt::EditRole, row.id);
        item(r++, 1)->setData(Qt::EditRole, row.name);
    }

    // Add more rows to accommodate more data and set contents of them.
    insertRows(rows.mid(r));
}


void SelectionsTable::insertRow(const MacroMetadata &row)
{
    QList<MacroMetadata> rows;
    rows.append(row);
    insertRows(rows);
}


void SelectionsTable::insertRows(const QList<MacroMetadata> &rows)
{
    // Stop model signals so that we can efficiently bulk insert.
    model()->blockSignals(true);

    int r = rowCount();
    // Make room for new rows (add more rows to table).
    setRowCount(rowCount() + rows.size());
    foreach (MacroMetadata row, rows) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::EditRole, row.id);
        setItem(r, 0, item);
        setItem(r++, 1, new QTableWidgetItem(row.name));
    }

    // Notify model that it can now update after bulk insert.
    model()->blockSignals(false);
    model()->layoutChanged();

    // Make sure we deselect scrollbar slider if adding rows!
    if (rows.size() > 0) {
        QMouseEvent rEvent(QEvent::MouseButtonRelease, QPointF(10, 100), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(verticalScrollBar(), &rEvent);
    }
}


void SelectionsTable::removeSelectedRows()
{
    model()->blockSignals(true);
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    typedef QList<QTableWidgetSelectionRange>::iterator Iter;
    // Iterate every 2 items.
    for (Iter it = ranges.end(); it != ranges.begin();) {
        --it;
        for (int r = it->bottomRow(); r >= it->topRow(); r--) {
            removeRow(r);
        }
    }
    model()->blockSignals(false);
    model()->layoutChanged();
}


void SelectionsTable::handleVertScrollBarMovement(int trackPos)
{
    if (trackPos == verticalScrollBar()->maximum()) {
        emit scrollBarHitBottom();
    }
}


void SelectionsTable::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        emit leftDoubleClick();
    }
}

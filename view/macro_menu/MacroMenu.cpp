#include "MacroMenu.h"
#include "ui_MacroMenu.h"
#include "MacroMenuContextMenu.h"
#include "CustomDialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QDesktopWidget>


MacroMenu::MacroMenu(MacroMenuEventListener &eventListener, const QRegExp &macroNameRegex) :
    _eventListener(eventListener),
    ui(new Ui::MacroMenu),
    _nameValidator(macroNameRegex)
{
    ui->setupUi(this);

    // Position the Macro Menu on top right corner of screen.
    QRect screenRect = QApplication::desktop()->screenGeometry();
    int x = (screenRect.right() - (this->window()->width() + 10));
    move(x, 2);

    // Setup of child components of Macro Menu window.
    ui->selectionEdit->setValidator(&_nameValidator);
    ui->selectionTable->setSortOrder(ID_ASC);

    // Initiate all filters and listeners in all components of Macro Menu window.
    initEventFilters();
    initListeners();
}


MacroMenu::~MacroMenu()
{
    delete ui;
}


void MacroMenu::refresh(const QList<MacroMetadata> &macroMetaList, MacroMetadataSortOrder sortOrder, bool selectFirst)
{
    // Just refresh all components of the Main Menu and show.
    ui->selectionEdit->clear();
    ui->selectionTable->setSortOrder(sortOrder);
    ui->selectionTable->refresh(macroMetaList);
    // If we should select the first row and we have a row.
    if (selectFirst && ui->selectionTable->rowCount() > 0) {
        ui->selectionTable->selectRow(0);
        int firstRowId = ui->selectionTable->getSelectedRowIds().first();
        ui->selectionEdit->setText(QString::number(firstRowId));
    }
    refreshButtonStates();
    ui->selectionTable->clearSelection();
    show();
}


void MacroMenu::show()
{
    // Bring the window to forefront and focus.
    QMainWindow::showNormal();
    QMainWindow::activateWindow();
    // Refresh what our selection is based off of contents of selection edit.
    if (ui->selectionEdit->text() != "") {
        ui->selectionTable->selectRow(ui->selectionEdit->text().toInt());
    }
    else {
        ui->selectionTable->clearSelection();
    }
    ui->selectionEdit->setFocus();
    ui->selectionEdit->selectAll();
}


void MacroMenu::hide()
{
    QMainWindow::showMinimized();
    QMainWindow::hide();
}


void MacroMenu::handleSelectionEditChange(const QString &text)
{
    bool isFilterInt;
    text.toInt(&isFilterInt);
    // Determine sort order based off of contents of selection edit filter text.
    MacroMetadataSortOrder sortOrder = ui->selectionEdit->text().size() == 0 ? ui->selectionTable->getSortOrder() :
                                                                               (isFilterInt ? ID_ASC : NAME_ASC);
    ui->selectionTable->setSortOrder(sortOrder);

    // Request the Macro Metadata using the new filter.
    QList<MacroMetadata> macroMetaList = _eventListener.requestMacroMetadataWithFilter(text, 0, sortOrder);
    ui->selectionTable->refresh(macroMetaList);
    if (text != "" && ui->selectionTable->rowCount() > 0) {
        // Select first row only if text is id or name of first row.
        MacroMetadata firstRow = ui->selectionTable->getRowAt(0);
        if (firstRow.id == text.toInt() || firstRow.name == text) {
            ui->selectionTable->clearSelection();
            ui->selectionTable->selectRow(0);
        }
    }
    refreshButtonStates();
}


void MacroMenu::handleTableSort()
{
    QString filter = ui->selectionEdit->text();
    int numMacrosInTable = ui->selectionTable->rowCount();

    // Request Macros in sorted order.
    QList<MacroMetadata> macroMetaList = _eventListener.requestMacroMetadataWithFilter(filter, 0, numMacrosInTable,
                                                                                       ui->selectionTable->getSortOrder());
    ui->selectionTable->refresh(macroMetaList);
    refreshButtonStates();
}


void MacroMenu::refreshButtonStates()
{
    // Disable or enable buttons based off of new state of selected Macros in table.
    bool singleSelectionExists = ui->selectionTable->getNumSelectedRows() == 1;
    ui->runButton->setEnabled(singleSelectionExists);
}


void MacroMenu::handleContextMenu()
{
    MacroMenuContextMenu contextMenu("Context Menu", this);
    connect(&contextMenu.runAction,     SIGNAL(triggered()), this, SLOT(activateMacro()));
    connect(&contextMenu.createAction,  SIGNAL(triggered()), this, SLOT(createNewMacro()));
    connect(&contextMenu.renameAction,  SIGNAL(triggered()), this, SLOT(renameMacro()));
    connect(&contextMenu.editAction,    SIGNAL(triggered()), this, SLOT(editMacros()));
    connect(&contextMenu.deleteAction,  SIGNAL(triggered()), this, SLOT(removeMacros()));
    connect(&contextMenu.copyAction,    SIGNAL(triggered()), this, SLOT(copyMacros()));

    // Check to see which items should be disabled based off of number of rows selected.
    if (ui->selectionTable->getNumSelectedRows() != 1) {
        contextMenu.runAction.setEnabled(false);
        contextMenu.renameAction.setEnabled(false);
    }
    if (ui->selectionTable->getNumSelectedRows() < 1) {
        contextMenu.editAction.setEnabled(false);
        contextMenu.deleteAction.setEnabled(false);
        contextMenu.copyAction.setEnabled(false);
    }

    contextMenu.exec(QCursor::pos());
}


void MacroMenu::activateMacro()
{
    int numSelectedRows = ui->selectionTable->getNumSelectedRows();
    // Make sure only single selection has been made.
    if (numSelectedRows == 1) {
        int id = ui->selectionTable->getSelectedRowIds().first();
        _eventListener.activateMacro(id);
    }
    // Make sure selectionEdit has entry and it refers to some first element in selectionTable.
    else if (numSelectedRows == 0 && ui->selectionEdit->text().size() > 0 && ui->selectionTable->rowCount() > 0) {
        ui->selectionTable->selectRow(0);
        activateMacro();
    }
}


void MacroMenu::createNewMacro()
{
    // Show dialog to get new Macro name (includes _nameValidator filter from Model).
    CustomDialog createDialog(this, "Input a name for the new Macro", &_nameValidator);
    int resultCreateDialog = createDialog.exec();

    // Only continue Macro creation if the user did not cancel dialog!
    if (resultCreateDialog == QDialog::Accepted) {
        QString name = createDialog.getText();
        // This should not fail b/c we have a regex filter (_nameValidator) that we use in the createDialog from the Model!
        _eventListener.createNewMacro(name);
    }
}


void MacroMenu::renameMacro()
{
    if (ui->selectionTable->getNumSelectedRows() == 1) {
        int id = ui->selectionTable->getSelectedRowIds().first();

        // Show dialog to get new Macro name (includes _nameValidator filter from Model).
        CustomDialog renameDialog(this, "Input a new name for the Macro", &_nameValidator);
        int resultRenameDialog = renameDialog.exec();

        // Only continue Macro creation if the user did not cancel dialog!
        if (resultRenameDialog == QDialog::Accepted) {
            QString name = renameDialog.getText();
            // This should not fail b/c we have a regex filter (_nameValidator) that we use in the createDialog from the Model!
            _eventListener.renameMacro(id, name);
            QTableWidgetItem *item = ui->selectionTable->selectedItems().last();
            item->setText(name);
        }
    }
}


void MacroMenu::editMacros()
{
    if (ui->selectionTable->getNumSelectedRows() > 0) {
        QList<int> ids = ui->selectionTable->getSelectedRowIds();
        _eventListener.editMacros(ids);
    }
}


void MacroMenu::removeMacros()
{
    int removeCnt = ui->selectionTable->getNumSelectedRows();
    if (removeCnt > 0) {
        // Delete confirm popup.
        QMessageBox::StandardButton confirmDeleteReply;
        confirmDeleteReply = QMessageBox::question(this, "Confirm Delete",
                                                   "Are you sure you wish to permanently delete the selected macro(s)?",
                                                   QMessageBox::Yes | QMessageBox::No);

        // Make sure that the user wants to permanently delete selected Macros!
        if (confirmDeleteReply == QMessageBox::Yes) {
            // Get selected rows and remove from DB & UI.
            QList<int> ids = ui->selectionTable->getSelectedRowIds();
            _eventListener.removeMacros(ids);
            ui->selectionTable->removeSelectedRows();

            // Get filter and settings associated with requesting replacement Macro Metadata.
            QString filter = ui->selectionEdit->text();
            int offset = ui->selectionTable->rowCount();
            MacroMetadataSortOrder sortOrder = ui->selectionTable->getSortOrder();

            // Replace the removed Macro Metadata rows if more are available.
            QList<MacroMetadata> macroMetaList = _eventListener.requestMacroMetadataWithFilter(filter, offset, removeCnt, sortOrder);
            ui->selectionTable->insertRows(macroMetaList);
        }
    }
    refreshButtonStates();
}


void MacroMenu::copyMacros()
{
    QString idListStr;

    if (ui->selectionTable->getNumSelectedRows() >= 1) {
        QList<int> sourceCopyIds = ui->selectionTable->getSelectedRowIds();
        QList<MacroMetadata> copyResult = _eventListener.copyMacros(sourceCopyIds);

        // Generate the copy result ID string that will be entered in the selection edit.
        foreach (MacroMetadata result, copyResult) {
            idListStr += QString::number(result.id) + " ";
        }
        ui->selectionEdit->setText(idListStr.trimmed());

        // Refresh table to only contain the results of the copy.
        ui->selectionTable->setSortOrder(ID_ASC);
        ui->selectionTable->refresh(copyResult);
    }
}


void MacroMenu::handleScrollHitBottom()
{
    // Get all selection settings and filters before requesting more Macro Metadata.
    QString filter = ui->selectionEdit->text();
    int offset = ui->selectionTable->rowCount();
    MacroMetadataSortOrder sortOrder = ui->selectionTable->getSortOrder();

    QList<MacroMetadata> macroMetaList = _eventListener.requestMacroMetadataWithFilter(filter, offset, sortOrder);
    // If we got more Macro Metadata, then insert into table after delay.
    if (macroMetaList.size() > 0) {
        QThread::msleep(150);
        ui->selectionTable->insertRows(macroMetaList);
    }
}


void MacroMenu::initEventFilters()
{
    ui->centralWidget->installEventFilter(this);
    ui->selectionEdit->installEventFilter(this);
    ui->selectionTable->installEventFilter(this);
    ui->createButton->installEventFilter(this);
    ui->runButton->installEventFilter(this);
}


void MacroMenu::initListeners()
{
    connect(ui->selectionEdit, SIGNAL(textEdited(QString)), this, SLOT(handleSelectionEditChange(QString)));
    connect(ui->selectionTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(refreshButtonStates()));
    connect(ui->selectionTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(handleTableSort()));
    connect(ui->selectionTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleContextMenu()));
    connect(ui->selectionTable, SIGNAL(leftDoubleClick()), this, SLOT(activateMacro()));
    connect(ui->selectionTable, SIGNAL(scrollBarHitBottom()), this, SLOT(handleScrollHitBottom()));
    connect(ui->runButton, SIGNAL(clicked(bool)), this, SLOT(activateMacro()));
    connect(ui->createButton, SIGNAL(clicked(bool)), this, SLOT(createNewMacro()));
}


bool MacroMenu::eventFilter(QObject *obj, QEvent *event)
{
    bool stopEvent = false;

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        stopEvent = filterGeneralKeyEvents(keyEvent);
        if (!stopEvent && obj == ui->selectionTable) {
            stopEvent = filterSelectionTableKeyEvents(keyEvent);
        }
    }

    return stopEvent;
}


bool MacroMenu::filterGeneralKeyEvents(const QKeyEvent *keyEvent)
{
    int key = keyEvent->key();
    int mods = keyEvent->modifiers();

    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        activateMacro();
    }
    else if (key == Qt::Key_Plus && mods == Qt::CTRL) { // ctrl++
        createNewMacro();
    }
    else if (key == Qt::Key_E && mods == Qt::CTRL) { // ctrl+e
        editMacros();
    }
    else if (key == Qt::Key_Escape) {
        close();
    }
    else {
        return false; // Do NOT stop processing of key event.
    }

    return true; // Stop processing of key event.
}


bool MacroMenu::filterSelectionTableKeyEvents(const QKeyEvent *keyEvent)
{
    int key = keyEvent->key();
    int mods = keyEvent->modifiers();

    if (key == Qt::Key_Delete) {
        removeMacros();
    }
    else if (key == Qt::Key_C && mods == Qt::CTRL) {
        copyMacros();
    }
    else {
        return false; // Do NOT stop processing of key event.
    }

    return true; // Stop processing of key event.
}

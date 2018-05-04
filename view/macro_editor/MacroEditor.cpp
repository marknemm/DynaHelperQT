#include "MacroEditor.h"
#include "ui_MacroEditor.h"
#include "MacroEditorContextMenu.h"
#include <QDebug>
#include <QMessageBox>


MacroEditor::MacroEditor(MacroEditorEventListener &editorEventListener, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MacroEditor),
    _editorEventListener(editorEventListener)
{
    ui->setupUi(this);
    connect(ui->actionSaveButton, SIGNAL(triggered(bool)), this, SLOT(saveEvents()));
    connect(ui->actionUndoButton, SIGNAL(triggered(bool)), this, SLOT(undoLastEventChange()));
    connect(ui->actionRedoButton, SIGNAL(triggered(bool)), this, SLOT(redoLastEventChange()));
    connect(ui->macroEventsTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleContextMenu()));
    ui->macroEventsTable->setEventListener(_editorEventListener);
}


MacroEditor::~MacroEditor()
{
    delete ui;
}


void MacroEditor::refresh(const QList<MacroEvent> &macroEvents, const QList<int> *macroIds)
{
    // Reset contents and show.
    ui->macroEventsTable->setRowCount(0);
    if (macroIds != nullptr) {
        setMacroIds(*macroIds);
    }
    setMacroEvents(macroEvents);
    show();
    ui->macroEventsTable->refreshColumnWidths(macroEvents);
}


void MacroEditor::show()
{
    // Make sure we resize the table columns to fit their contents
    ui->macroEventsTable->resizeColumnsToContents();

    // Bring the window to forefront and focus.
    QMainWindow::showNormal();
    QMainWindow::activateWindow();
    ui->centralWidget->setFocus();
}


void MacroEditor::setMacroIds(const QList<int> &macroIds)
{
    QString macroIdStr = "Editing Macro(s): ";

    // Build macro ID string for Macro IDs Label.
    foreach (int id, macroIds) {
        macroIdStr += QString::number(id) + ", ";
    }
    // Remove extra ", " at end.
    macroIdStr = macroIdStr.left(macroIdStr.length() - 2);

    ui->macroIdsLabel->setText(macroIdStr);
}


void MacroEditor::setMacroEvents(const QList<MacroEvent> &macroEvents)
{
    ui->macroEventsTable->addMacroEvents(macroEvents);
}


void MacroEditor::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    QList<MacroEvent> macroEvents = _editorEventListener.getLatestMacroEvents();
    ui->macroEventsTable->refreshColumnWidths(macroEvents);
}


void MacroEditor::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::Yes;

    // If we have unsaved changes, then ask user if they want to exit without saving.
    if (_editorEventListener.hasUnsavedChanges()) {
        reply = QMessageBox::question(this, "Editor Exit", "Are you sure you wish to exit without saving?",
                                      QMessageBox::Yes | QMessageBox::No);
    }

    // Make sure we want to exit if we haven't saved our latest changes!
    if (reply == QMessageBox::Yes) {
        QMainWindow::closeEvent(event);
        emit closed(); // Make sure we let controller know we have closed.
    }
    else {
        event->ignore();
    }
}


void MacroEditor::handleContextMenu()
{
    // Setup custom context menu and associated event handling connections.
    MacroEditorContextMenu contextMenu("Macro Editor Context Menu", this);
    connect(&contextMenu.insertBeforeAction,  SIGNAL(triggered()), this, SLOT(insertEventsBefore()));
    connect(&contextMenu.insertAfterAction,   SIGNAL(triggered()), this, SLOT(insertEventsAfter()));
    connect(&contextMenu.copyAction,          SIGNAL(triggered()), this, SLOT(copyEvents()));
    connect(&contextMenu.deleteAction,        SIGNAL(triggered()), this, SLOT(deleteEvents()));

    // Check to see which items should be disabled based off of number of rows selected.
    int selectedRowNum = ui->macroEventsTable->getNumSelectedRows();
    if (selectedRowNum > 1) {
        contextMenu.insertBeforeAction.setEnabled(false);
        contextMenu.insertAfterAction.setEnabled(false);
        contextMenu.copyAction.setEnabled(false);
    }
    else if (selectedRowNum < 1) {
        contextMenu.deleteAction.setEnabled(false);
        contextMenu.copyAction.setEnabled(false);
    }
    else if (ui->macroEventsTable->isSelectedRowDisabled()) {
        contextMenu.copyAction.setEnabled(false);
    }

    contextMenu.exec(QCursor::pos());
}


void MacroEditor::insertEventsBefore()
{
    int eventInd = 0;
    if (ui->macroEventsTable->getSelectedEventInds().size() > 0) {
        eventInd = ui->macroEventsTable->getSelectedEventInds().first();
    }
    _editorEventListener.insertEventsAt(eventInd);
}


void MacroEditor::insertEventsAfter()
{
    int eventInd = 0;
    if (ui->macroEventsTable->getSelectedEventInds().size() > 0) {
        eventInd = ui->macroEventsTable->getSelectedEventInds().first() + 1;
    }
    _editorEventListener.insertEventsAt(eventInd);
}


void MacroEditor::copyEvents()
{
    QList<int> eventInds = ui->macroEventsTable->getSelectedEventInds();
    _editorEventListener.copyEvents(eventInds);
}


void MacroEditor::deleteEvents()
{
    QList<int> eventInds = ui->macroEventsTable->getSelectedEventInds();
    _editorEventListener.deleteEvents(eventInds);
}


void MacroEditor::saveEvents()
{
    _editorEventListener.saveEvents();
}


void MacroEditor::undoLastEventChange()
{
    _editorEventListener.undoLastEventChange();
}


void MacroEditor::redoLastEventChange()
{
    _editorEventListener.redoLastEventChange();
}

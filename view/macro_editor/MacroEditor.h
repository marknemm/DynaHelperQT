#ifndef MACROEDITOR_H
#define MACROEDITOR_H


#include <QMainWindow>
#include <QCloseEvent>
#include <QList>
#include <model/MacroEvent.h>
#include "controller/macro_editor/MacroEditorEventListener.h"


namespace Ui {
class MacroEditor;
}


class MacroEditor : public QMainWindow
{
    Q_OBJECT

public:

    explicit MacroEditor(MacroEditorEventListener &editorEventListener, QWidget *parent = 0);
    ~MacroEditor();

    /**
     * @brief refresh
     * Refreshes the editor to display specific Macro IDs and Macro Events.
     * Then, shows the editor normally.
     * @param macroEvents
     * The list of Macro Events to display.
     * @param macroIds (OPTIONAL)
     * The list of Macro IDs to display.
     */
    void refresh(const QList<MacroEvent> &macroEvents, const QList<int> *macroIds = nullptr);

    /**
     * @brief show
     * Shows the view window and focuses it.
     */
    void show();


signals:

    /**
     * @brief closed
     * Emmitted whenever the view window is closed.
     */
    void closed();


private:

    Ui::MacroEditor *ui;
    /**
     * @brief _editorEventListener
     * An event listener for input events in the Macro Editor view.
     * Processes input events by handing them off to the model business logic component.
     */
    MacroEditorEventListener &_editorEventListener;


    /**
     * @brief setMacroIds
     * Sets the IDs of the Macros that are being edited.
     * @param macroIds
     * A list of the Macro IDs that are being edited.
     */
    void setMacroIds(const QList<int> &macroIds);

    /**
     * @brief setMacroEvents
     * Sets the Macro Events that the Macro Editor view is to display.
     * @param macroEvents
     * The Macro Events to display.
     */
    void setMacroEvents(const QList<MacroEvent> &macroEvents);


    /**
     * @brief resizeEvent
     * Called whenever the window is resized.
     * @param event
     * The resize event information.
     */
    void resizeEvent(QResizeEvent *event) override;


    /**
     * @brief closeEvent
     * Intercept the close event for this window.
     * @param event
     * The close event information.
     */
    void closeEvent(QCloseEvent *event) override;


private slots:

    /**
     * @brief handleContextMenu
     * Handles right click context menu.
     */
    void handleContextMenu();


    /**
     * @brief insertEventBefore
     * Inserts a series of Macro Event(s) before the selected event.
     * If no event is selected, then it inserts an event at the beginning of the list of Macro Events.
     */
    void insertEventsBefore();

    /**
     * @brief insertEventsAfter
     * Inserts a series of Macro Event(s) after the selected event.
     * If no event is selected, then it inserts an event at the end of the list of Macro Events.
     */
    void insertEventsAfter();

    /**
     * @brief copyEvents
     * Copies all of the selected Macro Events and inserts the copies immediately after their sources.
     */
    void copyEvents();

    /**
     * @brief deleteEvents
     * Deletes all selected Macro Events.
     */
    void deleteEvents();

    /**
     * @brief saveEvents
     * Handles save button press.
     */
    void saveEvents();

    /**
     * @brief undoLastEventChange
     * Handles undo button press or key combo (ctrl + z)
     */
    void undoLastEventChange();

    /**
     * @brief redoLastEventChange
     * Handles redo button press or key combo (ctrl + y)
     */
    void redoLastEventChange();
};


#endif // MACROEDITOR_H

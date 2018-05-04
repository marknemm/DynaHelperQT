#ifndef MAINMENU_H
#define MAINMENU_H


#include "controller/macro_menu/MacroMenuEventListener.h"
#include <QMainWindow>
#include <QString>
#include <QEvent>
#include <QKeyEvent>
#include <QRegExp>
#include <QRegExpValidator>
#include <memory>


namespace Ui {
class MacroMenu;
}


/**
 * @brief The MacroMenu class
 * The main menu GUI view class.
 */
class MacroMenu : public QMainWindow
{
    Q_OBJECT

public:

    explicit MacroMenu(MacroMenuEventListener &controllerInterface, const QRegExp &macroNameRegex);
    ~MacroMenu();

    /**
     * @brief refresh
     * Refreshes the GUI and displays the view window. Also involves repopulating the view with model data.
     * @param macroMetaList
     * List of Macro metadata elements to diplay upon refresh.
     * @param sortOrder
     * The order that the refresh Macro Metadata is coming in.
     * @param selectFirst
     * True if the first item in the macroMetaList should be set select, false if no selection should be made.
     */
    void refresh(const QList<MacroMetadata> &macroMetaList, MacroMetadataSortOrder sortOrder, bool selectFirst);


    /**
     * @brief display
     * Everything in refresh() excluding repopulating the selections table.
     */
    void show();

    /**
     * @brief hide
     * Hides the Main Menu window.
     */
    void hide();


private slots:

    /**
     * @brief handleSelectionEditChange
     * Handles a change in the selectionEdit text.
     * @param text
     * The new text after a change in the selectionEdit.
     */
    void handleSelectionEditChange(const QString &text);


    /**
     * @brief handleTableSort
     * Handler that responds to sorting event in selections table.
     */
    void handleTableSort();


    /**
     * @brief refreshButtonStates
     * Checks what the enable states of UI buttons should be and refreshes them.
     */
    void refreshButtonStates();


    /**
     * @brief handleContextMenu
     * Receives signal whenever a context menu is requested.
     */
    void handleContextMenu();


    /**
     * @brief activateMacro
     * Inits the activation of a macro if UI state permits it.
     */
    void activateMacro();

    /**
     * @brief createNewMacro
     * Inits the creation of a macro if UI state permits it.
     */
    void createNewMacro();

    /**
     * @brief renameMacro
     * Inits the renaming of a macro if UI state permits it.
     */
    void renameMacro();

    /**
     * @brief editMacros
     * Inits the editing of macro(s) if UI state permits it.
     */
    void editMacros();

    /**
     * @brief removeMacros
     * Inits the deletion of macro(s) if UI state permits it.
     */
    void removeMacros();

    /**
     * @brief copyMacros
     * Inits the copying of macro(s) if UI state permits it.
     */
    void copyMacros();


    /**
     * @brief handleScrollHitBottom
     * Requests more macros to display upon scrollbar hitting bottom.
     */
    void handleScrollHitBottom();

private:

    Ui::MacroMenu *ui;

    /**
     * @brief _controllerInterface
     * Interface to the controller for the Main Menu. Used to more loosely couple the
     * Main Menu Controller with the Main Menu View.
     */
    MacroMenuEventListener &_eventListener;

    /**
     * @brief _nameValidator
     * Validator for Macro names.
     */
    QRegExpValidator _nameValidator;


    /**
     * @brief initEventFilters
     * Initializes event filters in all UI components.
     */
    void initEventFilters();

    /**
     * @brief initListeners
     * Initializes listeners in all UI components.
     */
    void initListeners();


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
     * @brief filterGeneralKeyEvents
     * General filter for keyboard events that are not specific to any element in the main menu UI.
     * @param keyEvent
     * The key event to potentially be filtered.
     * @return
     * true if should stop further handling of event, false otherwise.
     */
    bool filterGeneralKeyEvents(const QKeyEvent *keyEvent);

    /**
     * @brief filterSelectionTableKeyEvents
     * Filters certain keyboard inputs designated specifically for the selectionTable UI element.
     * @param keyEvent
     * The keyboard event.
     * @return
     * true if should stop further handling of event, false otherwise.
     */
    bool filterSelectionTableKeyEvents(const QKeyEvent *keyEvent);
};

#endif // MAINMENU_H

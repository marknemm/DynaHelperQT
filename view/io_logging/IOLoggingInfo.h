#ifndef IOLOGGINGINFO_H
#define IOLOGGINGINFO_H

#include <QMainWindow>

namespace Ui {
class IOLoggingInfo;
}

/**
 * @brief The IOLoggingInfo class
 * Semi-transparent floating window for logging purposes.
 */
class IOLoggingInfo : public QMainWindow
{
    Q_OBJECT

public:

    explicit IOLoggingInfo(QWidget *parent = 0);
    ~IOLoggingInfo();

    /**
     * @brief setHeader
     * Updates the event info display's header (instructions).
     * @param header
     * The new header instruction text to display.
     */
    void setHeader(const QString &header);

    /**
     * @brief updateEventInfo
     * Updates the event info dispaly.
     * @param eventInfo The string to update to.
     */
    void updateEventInfo(const QString &eventInfo);

private:

    Ui::IOLoggingInfo *ui;
    /**
     * @brief _corner
     * Represents the corner that the window is being displayed in.
     */
    QString _corner;

    /**
     * @brief initFilters
     * Initializes the event filters in all GUI components.
     */
    void initFilters();
    /**
     * @brief eventFilter
     * General event filter for main menu UI.
     * @param obj The UI element that would normally receive the event.
     * @param event The event that is to be filtered.
     * @return true if should stop further handling of event, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;
    /**
     * @brief swapCorner
     * Swaps the corner that the IO logging info window is in.
     */
    void swapCorner();
};

#endif // IOLOGGINGINFO_H

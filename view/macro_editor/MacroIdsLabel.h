#ifndef MACROIDSLABEL_H
#define MACROIDSLABEL_H


#include <QWidget>
#include <QLabel>
#include <QResizeEvent>


/**
 * @brief The MacroIdsLabel class
 * Label that will contain an elided list of Macro IDs with tooltip set to the current text when elided.
 */
class MacroIdsLabel : public QLabel
{
    Q_OBJECT

public:

    explicit MacroIdsLabel(QWidget *parent = nullptr);


    /**
     * @brief setText
     * Sets the text of the label.
     * @param text
     * The text to be set.
     */
    void setText(const QString &text);

    /**
     * @brief nonElidedText
     * Gets the non-elided (full) text of the label.
     * @return
     * The non-elided text of the label.
     */
    QString nonElidedText() const;


    /**
     * @brief setToolTip
     * Sets the tooltip for this QLabel. Wraps the text given in HTML tags to give it rich text format
     * so that word wrapping will be used.
     * @param text
     * The text to set the tooltip to.
     */
    void setToolTip(const QString &text);

    /**
     * @brief toolTip
     * Gets the raw text of the tooltip (without any HTML tag wrappings).
     * @return
     * The raw text of the tooltip.
     */
    QString toolTip() const;


protected:

    /**
     * @brief resizeEvent
     * Handles resize events.
     * @param event
     * The resize event information.
     */
    void resizeEvent(QResizeEvent *event) override;
};


#endif // MACROIDSLABEL_H

#include "MacroIdsLabel.h"
#include <QFontMetrics>
#include <QRegExp>
#include <QDebug>


MacroIdsLabel::MacroIdsLabel(QWidget *parent)
    : QLabel(parent)
{}


void MacroIdsLabel::setText(const QString &text)
{
    int width = (QLabel::width() - 2);

    // Generate elided text for the label and set the label text to the elided text.
    QFontMetrics metrix(QLabel::font());
    QString elidedText = metrix.elidedText(text, Qt::ElideRight, width);
    QLabel::setText(elidedText);

    // If the elided text is not the full text, then set the tooltip to non-elided (full) text.
    (elidedText != text) ? QLabel::setToolTip("<FONT>" + text + "</FONT>") : // <FONT> for rich text for word wrap!
                           QLabel::setToolTip("");
}


QString MacroIdsLabel::nonElidedText() const
{
    return (QLabel::toolTip().length() > 0) ? toolTip()       // If it is in the tooltip because the label text is elided.
                                            : QLabel::text(); // Else the label text is not elided.
}


void MacroIdsLabel::setToolTip(const QString &text)
{
    // Wrap in HTML tags for rich text format (word wrapping)!
    (text.size() > 0) ? QLabel::setToolTip("<FONT>" + text + "</FONT>")
                      : QLabel::setToolTip("");
}


QString MacroIdsLabel::toolTip() const
{
    // Strip off any HTML tags.
    return QLabel::toolTip().remove(QRegExp("<[^>]*>"));
}


void MacroIdsLabel::resizeEvent(QResizeEvent *event)
{
    // Do resize and set the text anew for updated elided text.
    QLabel::resizeEvent(event);
    setText(nonElidedText()); // The tooltip will contain the non-elided text.
}

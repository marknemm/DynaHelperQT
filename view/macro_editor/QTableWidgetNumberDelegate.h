#ifndef QTABLEWIDGETNUMBERDELEGATE_H
#define QTABLEWIDGETNUMBERDELEGATE_H


#include <QItemDelegate>
#include <QWidget>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QList>


/**
 * @brief The QTableWidgetNumberDelegate class
 * Delegate for numeric type QTableWidgetItem containers.
 * Used primarily to ensure that only numbers can be entered in these cells.
 */
class QTableWidgetNumberDelegate : public QItemDelegate
{

public:

    explicit QTableWidgetNumberDelegate(const QList<int> &numberColIndexes);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:

    QList<int> _numberColIndexes;
};


#endif // QTABLEWIDGETNUMBERDELEGATE_H

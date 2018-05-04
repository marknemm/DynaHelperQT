#include "QTableWidgetNumberDelegate.h"
#include <QIntValidator>
#include <QLineEdit>


QTableWidgetNumberDelegate::QTableWidgetNumberDelegate(const QList<int> &numberColIndexes)
    : QItemDelegate(),
      _numberColIndexes(numberColIndexes)
{}


QWidget* QTableWidgetNumberDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Are we editing numeric fields?
    if (_numberColIndexes.contains(index.column())) {
        QLineEdit *lineEdit = new QLineEdit(parent);
        QIntValidator *validator = new QIntValidator(0, 9999, lineEdit);
        lineEdit->setValidator(validator);
        return lineEdit;
    }
    // Generic editor otherwise.
    else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

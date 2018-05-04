/****************************************************************************
** Meta object code from reading C++ file 'MacroEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/macro_editor/MacroEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MacroEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MacroEditor_t {
    QByteArrayData data[11];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MacroEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MacroEditor_t qt_meta_stringdata_MacroEditor = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MacroEditor"
QT_MOC_LITERAL(1, 12, 6), // "closed"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 17), // "handleContextMenu"
QT_MOC_LITERAL(4, 38, 18), // "insertEventsBefore"
QT_MOC_LITERAL(5, 57, 17), // "insertEventsAfter"
QT_MOC_LITERAL(6, 75, 10), // "copyEvents"
QT_MOC_LITERAL(7, 86, 12), // "deleteEvents"
QT_MOC_LITERAL(8, 99, 10), // "saveEvents"
QT_MOC_LITERAL(9, 110, 19), // "undoLastEventChange"
QT_MOC_LITERAL(10, 130, 19) // "redoLastEventChange"

    },
    "MacroEditor\0closed\0\0handleContextMenu\0"
    "insertEventsBefore\0insertEventsAfter\0"
    "copyEvents\0deleteEvents\0saveEvents\0"
    "undoLastEventChange\0redoLastEventChange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MacroEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MacroEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MacroEditor *_t = static_cast<MacroEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->handleContextMenu(); break;
        case 2: _t->insertEventsBefore(); break;
        case 3: _t->insertEventsAfter(); break;
        case 4: _t->copyEvents(); break;
        case 5: _t->deleteEvents(); break;
        case 6: _t->saveEvents(); break;
        case 7: _t->undoLastEventChange(); break;
        case 8: _t->redoLastEventChange(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MacroEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MacroEditor::closed)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MacroEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MacroEditor.data,
      qt_meta_data_MacroEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MacroEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MacroEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MacroEditor.stringdata0))
        return static_cast<void*>(const_cast< MacroEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MacroEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void MacroEditor::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

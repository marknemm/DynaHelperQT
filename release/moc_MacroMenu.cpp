/****************************************************************************
** Meta object code from reading C++ file 'MacroMenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/macro_menu/MacroMenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MacroMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MacroMenu_t {
    QByteArrayData data[14];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MacroMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MacroMenu_t qt_meta_stringdata_MacroMenu = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MacroMenu"
QT_MOC_LITERAL(1, 10, 25), // "handleSelectionEditChange"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 4), // "text"
QT_MOC_LITERAL(4, 42, 15), // "handleTableSort"
QT_MOC_LITERAL(5, 58, 19), // "refreshButtonStates"
QT_MOC_LITERAL(6, 78, 17), // "handleContextMenu"
QT_MOC_LITERAL(7, 96, 13), // "activateMacro"
QT_MOC_LITERAL(8, 110, 14), // "createNewMacro"
QT_MOC_LITERAL(9, 125, 11), // "renameMacro"
QT_MOC_LITERAL(10, 137, 10), // "editMacros"
QT_MOC_LITERAL(11, 148, 12), // "removeMacros"
QT_MOC_LITERAL(12, 161, 10), // "copyMacros"
QT_MOC_LITERAL(13, 172, 21) // "handleScrollHitBottom"

    },
    "MacroMenu\0handleSelectionEditChange\0"
    "\0text\0handleTableSort\0refreshButtonStates\0"
    "handleContextMenu\0activateMacro\0"
    "createNewMacro\0renameMacro\0editMacros\0"
    "removeMacros\0copyMacros\0handleScrollHitBottom"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MacroMenu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x08 /* Private */,
       4,    0,   72,    2, 0x08 /* Private */,
       5,    0,   73,    2, 0x08 /* Private */,
       6,    0,   74,    2, 0x08 /* Private */,
       7,    0,   75,    2, 0x08 /* Private */,
       8,    0,   76,    2, 0x08 /* Private */,
       9,    0,   77,    2, 0x08 /* Private */,
      10,    0,   78,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,
      12,    0,   80,    2, 0x08 /* Private */,
      13,    0,   81,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
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

void MacroMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MacroMenu *_t = static_cast<MacroMenu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleSelectionEditChange((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->handleTableSort(); break;
        case 2: _t->refreshButtonStates(); break;
        case 3: _t->handleContextMenu(); break;
        case 4: _t->activateMacro(); break;
        case 5: _t->createNewMacro(); break;
        case 6: _t->renameMacro(); break;
        case 7: _t->editMacros(); break;
        case 8: _t->removeMacros(); break;
        case 9: _t->copyMacros(); break;
        case 10: _t->handleScrollHitBottom(); break;
        default: ;
        }
    }
}

const QMetaObject MacroMenu::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MacroMenu.data,
      qt_meta_data_MacroMenu,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MacroMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MacroMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MacroMenu.stringdata0))
        return static_cast<void*>(const_cast< MacroMenu*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MacroMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

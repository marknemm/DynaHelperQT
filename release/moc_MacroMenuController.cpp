/****************************************************************************
** Meta object code from reading C++ file 'MacroMenuController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/macro_menu/MacroMenuController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MacroMenuController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MacroMenuController_t {
    QByteArrayData data[10];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MacroMenuController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MacroMenuController_t qt_meta_stringdata_MacroMenuController = {
    {
QT_MOC_LITERAL(0, 0, 19), // "MacroMenuController"
QT_MOC_LITERAL(1, 20, 23), // "assumeControlFromParent"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 11), // "Controller*"
QT_MOC_LITERAL(4, 57, 16), // "parentController"
QT_MOC_LITERAL(5, 74, 22), // "assumeControlFromChild"
QT_MOC_LITERAL(6, 97, 17), // "SrcControllerInfo"
QT_MOC_LITERAL(7, 115, 17), // "srcControllerInfo"
QT_MOC_LITERAL(8, 133, 16), // "surrenderControl"
QT_MOC_LITERAL(9, 150, 12) // "handleHotKey"

    },
    "MacroMenuController\0assumeControlFromParent\0"
    "\0Controller*\0parentController\0"
    "assumeControlFromChild\0SrcControllerInfo\0"
    "srcControllerInfo\0surrenderControl\0"
    "handleHotKey"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MacroMenuController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       1,    0,   47,    2, 0x2a /* Public | MethodCloned */,
       5,    1,   48,    2, 0x0a /* Public */,
       5,    0,   51,    2, 0x2a /* Public | MethodCloned */,
       8,    0,   52,    2, 0x08 /* Private */,
       9,    0,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MacroMenuController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MacroMenuController *_t = static_cast<MacroMenuController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->assumeControlFromParent((*reinterpret_cast< Controller*(*)>(_a[1]))); break;
        case 1: _t->assumeControlFromParent(); break;
        case 2: _t->assumeControlFromChild((*reinterpret_cast< const SrcControllerInfo(*)>(_a[1]))); break;
        case 3: _t->assumeControlFromChild(); break;
        case 4: _t->surrenderControl(); break;
        case 5: _t->handleHotKey(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Controller* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MacroMenuController::staticMetaObject = {
    { &Controller::staticMetaObject, qt_meta_stringdata_MacroMenuController.data,
      qt_meta_data_MacroMenuController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MacroMenuController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MacroMenuController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MacroMenuController.stringdata0))
        return static_cast<void*>(const_cast< MacroMenuController*>(this));
    if (!strcmp(_clname, "MacroMenuEventListener"))
        return static_cast< MacroMenuEventListener*>(const_cast< MacroMenuController*>(this));
    return Controller::qt_metacast(_clname);
}

int MacroMenuController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Controller::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

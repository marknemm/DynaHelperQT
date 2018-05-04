/****************************************************************************
** Meta object code from reading C++ file 'MacroAddController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/macro_add/MacroAddController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MacroAddController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MacroAddController_t {
    QByteArrayData data[9];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MacroAddController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MacroAddController_t qt_meta_stringdata_MacroAddController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MacroAddController"
QT_MOC_LITERAL(1, 19, 23), // "assumeControlFromParent"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 9), // "macroName"
QT_MOC_LITERAL(4, 54, 11), // "Controller*"
QT_MOC_LITERAL(5, 66, 16), // "parentController"
QT_MOC_LITERAL(6, 83, 22), // "assumeControlFromChild"
QT_MOC_LITERAL(7, 106, 17), // "SrcControllerInfo"
QT_MOC_LITERAL(8, 124, 17) // "srcControllerInfo"

    },
    "MacroAddController\0assumeControlFromParent\0"
    "\0macroName\0Controller*\0parentController\0"
    "assumeControlFromChild\0SrcControllerInfo\0"
    "srcControllerInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MacroAddController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x0a /* Public */,
       1,    1,   34,    2, 0x2a /* Public | MethodCloned */,
       6,    1,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void MacroAddController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MacroAddController *_t = static_cast<MacroAddController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->assumeControlFromParent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< Controller*(*)>(_a[2]))); break;
        case 1: _t->assumeControlFromParent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->assumeControlFromChild((*reinterpret_cast< const SrcControllerInfo(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Controller* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MacroAddController::staticMetaObject = {
    { &Controller::staticMetaObject, qt_meta_stringdata_MacroAddController.data,
      qt_meta_data_MacroAddController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MacroAddController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MacroAddController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MacroAddController.stringdata0))
        return static_cast<void*>(const_cast< MacroAddController*>(this));
    return Controller::qt_metacast(_clname);
}

int MacroAddController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Controller::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

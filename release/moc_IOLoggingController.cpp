/****************************************************************************
** Meta object code from reading C++ file 'IOLoggingController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/io_logging/IOLoggingController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IOLoggingController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IOLoggingController_t {
    QByteArrayData data[12];
    char stringdata0[174];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IOLoggingController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IOLoggingController_t qt_meta_stringdata_IOLoggingController = {
    {
QT_MOC_LITERAL(0, 0, 19), // "IOLoggingController"
QT_MOC_LITERAL(1, 20, 23), // "assumeControlFromParent"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 9), // "insertInd"
QT_MOC_LITERAL(4, 55, 11), // "Controller*"
QT_MOC_LITERAL(5, 67, 16), // "parentController"
QT_MOC_LITERAL(6, 84, 24), // "surrenderControlToParent"
QT_MOC_LITERAL(7, 109, 20), // "deactivateDueToError"
QT_MOC_LITERAL(8, 130, 8), // "errorMsg"
QT_MOC_LITERAL(9, 139, 16), // "handleMacroEvent"
QT_MOC_LITERAL(10, 156, 11), // "MacroEvent&"
QT_MOC_LITERAL(11, 168, 5) // "event"

    },
    "IOLoggingController\0assumeControlFromParent\0"
    "\0insertInd\0Controller*\0parentController\0"
    "surrenderControlToParent\0deactivateDueToError\0"
    "errorMsg\0handleMacroEvent\0MacroEvent&\0"
    "event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IOLoggingController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x0a /* Public */,
       6,    2,   44,    2, 0x08 /* Private */,
       6,    1,   49,    2, 0x28 /* Private | MethodCloned */,
       6,    0,   52,    2, 0x28 /* Private | MethodCloned */,
       9,    1,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void IOLoggingController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IOLoggingController *_t = static_cast<IOLoggingController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->assumeControlFromParent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Controller*(*)>(_a[2]))); break;
        case 1: _t->surrenderControlToParent((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->surrenderControlToParent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->surrenderControlToParent(); break;
        case 4: _t->handleMacroEvent((*reinterpret_cast< MacroEvent(*)>(_a[1]))); break;
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

const QMetaObject IOLoggingController::staticMetaObject = {
    { &Controller::staticMetaObject, qt_meta_stringdata_IOLoggingController.data,
      qt_meta_data_IOLoggingController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IOLoggingController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IOLoggingController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IOLoggingController.stringdata0))
        return static_cast<void*>(const_cast< IOLoggingController*>(this));
    return Controller::qt_metacast(_clname);
}

int IOLoggingController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Controller::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

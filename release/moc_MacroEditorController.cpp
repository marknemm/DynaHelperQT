/****************************************************************************
** Meta object code from reading C++ file 'MacroEditorController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/macro_editor/MacroEditorController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MacroEditorController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MacroEditorController_t {
    QByteArrayData data[13];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MacroEditorController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MacroEditorController_t qt_meta_stringdata_MacroEditorController = {
    {
QT_MOC_LITERAL(0, 0, 21), // "MacroEditorController"
QT_MOC_LITERAL(1, 22, 23), // "assumeControlFromParent"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 10), // "QList<int>"
QT_MOC_LITERAL(4, 58, 8), // "macroIds"
QT_MOC_LITERAL(5, 67, 11), // "Controller*"
QT_MOC_LITERAL(6, 79, 16), // "parentController"
QT_MOC_LITERAL(7, 96, 22), // "assumeControlFromChild"
QT_MOC_LITERAL(8, 119, 17), // "SrcControllerInfo"
QT_MOC_LITERAL(9, 137, 17), // "srcControllerInfo"
QT_MOC_LITERAL(10, 155, 24), // "surrenderControlToParent"
QT_MOC_LITERAL(11, 180, 20), // "deactivateDueToError"
QT_MOC_LITERAL(12, 201, 8) // "errorMsg"

    },
    "MacroEditorController\0assumeControlFromParent\0"
    "\0QList<int>\0macroIds\0Controller*\0"
    "parentController\0assumeControlFromChild\0"
    "SrcControllerInfo\0srcControllerInfo\0"
    "surrenderControlToParent\0deactivateDueToError\0"
    "errorMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MacroEditorController[] = {

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
       7,    1,   44,    2, 0x0a /* Public */,
      10,    2,   47,    2, 0x08 /* Private */,
      10,    1,   52,    2, 0x28 /* Private | MethodCloned */,
      10,    0,   55,    2, 0x28 /* Private | MethodCloned */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   11,   12,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,

       0        // eod
};

void MacroEditorController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MacroEditorController *_t = static_cast<MacroEditorController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->assumeControlFromParent((*reinterpret_cast< const QList<int>(*)>(_a[1])),(*reinterpret_cast< Controller*(*)>(_a[2]))); break;
        case 1: _t->assumeControlFromChild((*reinterpret_cast< const SrcControllerInfo(*)>(_a[1]))); break;
        case 2: _t->surrenderControlToParent((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->surrenderControlToParent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->surrenderControlToParent(); break;
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
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    }
}

const QMetaObject MacroEditorController::staticMetaObject = {
    { &Controller::staticMetaObject, qt_meta_stringdata_MacroEditorController.data,
      qt_meta_data_MacroEditorController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MacroEditorController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MacroEditorController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MacroEditorController.stringdata0))
        return static_cast<void*>(const_cast< MacroEditorController*>(this));
    if (!strcmp(_clname, "MacroEditorEventListener"))
        return static_cast< MacroEditorEventListener*>(const_cast< MacroEditorController*>(this));
    return Controller::qt_metacast(_clname);
}

int MacroEditorController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

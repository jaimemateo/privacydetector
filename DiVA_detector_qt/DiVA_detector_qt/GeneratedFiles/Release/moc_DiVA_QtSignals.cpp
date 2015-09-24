/****************************************************************************
** Meta object code from reading C++ file 'DiVA_QtSignals.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DiVA_QtSignals.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DiVA_QtSignals.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DiVA_QtSignals_t {
    QByteArrayData data[9];
    char stringdata0[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DiVA_QtSignals_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DiVA_QtSignals_t qt_meta_stringdata_DiVA_QtSignals = {
    {
QT_MOC_LITERAL(0, 0, 14), // "DiVA_QtSignals"
QT_MOC_LITERAL(1, 15, 15), // "refresh_display"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "frame"
QT_MOC_LITERAL(4, 38, 27), // "refresh_display_comparation"
QT_MOC_LITERAL(5, 66, 9), // "_template"
QT_MOC_LITERAL(6, 76, 6), // "result"
QT_MOC_LITERAL(7, 83, 11), // "changeValue"
QT_MOC_LITERAL(8, 95, 5) // "value"

    },
    "DiVA_QtSignals\0refresh_display\0\0frame\0"
    "refresh_display_comparation\0_template\0"
    "result\0changeValue\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DiVA_QtSignals[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    2,   32,    2, 0x06 /* Public */,
       7,    1,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPixmap,    3,
    QMetaType::Void, QMetaType::QPixmap, QMetaType::QPixmap,    5,    6,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void DiVA_QtSignals::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DiVA_QtSignals *_t = static_cast<DiVA_QtSignals *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refresh_display((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 1: _t->refresh_display_comparation((*reinterpret_cast< const QPixmap(*)>(_a[1])),(*reinterpret_cast< const QPixmap(*)>(_a[2]))); break;
        case 2: _t->changeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DiVA_QtSignals::*_t)(const QPixmap & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DiVA_QtSignals::refresh_display)) {
                *result = 0;
            }
        }
        {
            typedef void (DiVA_QtSignals::*_t)(const QPixmap & , const QPixmap & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DiVA_QtSignals::refresh_display_comparation)) {
                *result = 1;
            }
        }
        {
            typedef void (DiVA_QtSignals::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DiVA_QtSignals::changeValue)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject DiVA_QtSignals::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DiVA_QtSignals.data,
      qt_meta_data_DiVA_QtSignals,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DiVA_QtSignals::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DiVA_QtSignals::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DiVA_QtSignals.stringdata0))
        return static_cast<void*>(const_cast< DiVA_QtSignals*>(this));
    return QObject::qt_metacast(_clname);
}

int DiVA_QtSignals::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DiVA_QtSignals::refresh_display(const QPixmap & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DiVA_QtSignals::refresh_display_comparation(const QPixmap & _t1, const QPixmap & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DiVA_QtSignals::changeValue(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

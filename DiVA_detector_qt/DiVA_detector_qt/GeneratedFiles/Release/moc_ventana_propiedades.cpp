/****************************************************************************
** Meta object code from reading C++ file 'ventana_propiedades.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ventana_propiedades.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ventana_propiedades.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ventana_propiedades_t {
    QByteArrayData data[23];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ventana_propiedades_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ventana_propiedades_t qt_meta_stringdata_ventana_propiedades = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ventana_propiedades"
QT_MOC_LITERAL(1, 20, 9), // "sendParam"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "hmax"
QT_MOC_LITERAL(4, 36, 4), // "hmin"
QT_MOC_LITERAL(5, 41, 4), // "wmax"
QT_MOC_LITERAL(6, 46, 4), // "wmin"
QT_MOC_LITERAL(7, 51, 1), // "Q"
QT_MOC_LITERAL(8, 53, 11), // "static_time"
QT_MOC_LITERAL(9, 65, 3), // "MHI"
QT_MOC_LITERAL(10, 69, 2), // "FG"
QT_MOC_LITERAL(11, 72, 8), // "varnoise"
QT_MOC_LITERAL(12, 81, 5), // "color"
QT_MOC_LITERAL(13, 87, 19), // "on_okButton_clicked"
QT_MOC_LITERAL(14, 107, 23), // "on_cancelButton_clicked"
QT_MOC_LITERAL(15, 131, 16), // "on_sliderchanged"
QT_MOC_LITERAL(16, 148, 5), // "valor"
QT_MOC_LITERAL(17, 154, 13), // "change_slider"
QT_MOC_LITERAL(18, 168, 16), // "on_scrollchanged"
QT_MOC_LITERAL(19, 185, 13), // "change_scroll"
QT_MOC_LITERAL(20, 199, 32), // "on_horizontalSlider_valueChanged"
QT_MOC_LITERAL(21, 232, 5), // "param"
QT_MOC_LITERAL(22, 238, 12) // "change_color"

    },
    "ventana_propiedades\0sendParam\0\0hmax\0"
    "hmin\0wmax\0wmin\0Q\0static_time\0MHI\0FG\0"
    "varnoise\0color\0on_okButton_clicked\0"
    "on_cancelButton_clicked\0on_sliderchanged\0"
    "valor\0change_slider\0on_scrollchanged\0"
    "change_scroll\0on_horizontalSlider_valueChanged\0"
    "param\0change_color"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ventana_propiedades[] = {

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
       1,   10,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   80,    2, 0x0a /* Public */,
      14,    0,   81,    2, 0x0a /* Public */,
      15,    1,   82,    2, 0x0a /* Public */,
      17,    0,   85,    2, 0x0a /* Public */,
      18,    1,   86,    2, 0x0a /* Public */,
      19,    0,   89,    2, 0x0a /* Public */,
      20,    1,   90,    2, 0x0a /* Public */,
      22,    0,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Int,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void,

       0        // eod
};

void ventana_propiedades::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ventana_propiedades *_t = static_cast<ventana_propiedades *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendParam((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7])),(*reinterpret_cast< double(*)>(_a[8])),(*reinterpret_cast< double(*)>(_a[9])),(*reinterpret_cast< int(*)>(_a[10]))); break;
        case 1: _t->on_okButton_clicked(); break;
        case 2: _t->on_cancelButton_clicked(); break;
        case 3: _t->on_sliderchanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->change_slider(); break;
        case 5: _t->on_scrollchanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->change_scroll(); break;
        case 7: _t->on_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->change_color(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ventana_propiedades::*_t)(int , int , int , int , int , int , double , double , double , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ventana_propiedades::sendParam)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ventana_propiedades::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ventana_propiedades.data,
      qt_meta_data_ventana_propiedades,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ventana_propiedades::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ventana_propiedades::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ventana_propiedades.stringdata0))
        return static_cast<void*>(const_cast< ventana_propiedades*>(this));
    return QDialog::qt_metacast(_clname);
}

int ventana_propiedades::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void ventana_propiedades::sendParam(int _t1, int _t2, int _t3, int _t4, int _t5, int _t6, double _t7, double _t8, double _t9, int _t10)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)), const_cast<void*>(reinterpret_cast<const void*>(&_t10)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

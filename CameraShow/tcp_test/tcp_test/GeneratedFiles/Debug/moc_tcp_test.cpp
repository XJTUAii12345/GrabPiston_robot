/****************************************************************************
** Meta object code from reading C++ file 'tcp_test.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tcp_test.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcp_test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tcp_test[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      24,    9,    9,    9, 0x08,
      43,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_tcp_test[] = {
    "tcp_test\0\0SendMessage()\0AcceptConnection()\0"
    "ReadClient()\0"
};

void tcp_test::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        tcp_test *_t = static_cast<tcp_test *>(_o);
        switch (_id) {
        case 0: _t->SendMessage(); break;
        case 1: _t->AcceptConnection(); break;
        case 2: _t->ReadClient(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData tcp_test::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject tcp_test::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_tcp_test,
      qt_meta_data_tcp_test, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tcp_test::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tcp_test::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tcp_test::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tcp_test))
        return static_cast<void*>(const_cast< tcp_test*>(this));
    return QWidget::qt_metacast(_clname);
}

int tcp_test::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

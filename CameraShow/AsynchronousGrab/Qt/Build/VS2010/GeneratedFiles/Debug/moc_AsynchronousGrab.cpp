/****************************************************************************
** Meta object code from reading C++ file 'AsynchronousGrab.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Source/AsynchronousGrab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AsynchronousGrab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AsynchronousGrab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      54,   47,   17,   17, 0x08,
      79,   72,   17,   17, 0x08,
     104,   17,   17,   17, 0x08,
     129,   17,   17,   17, 0x08,
     158,   17,   17,   17, 0x08,
     189,   17,   17,   17, 0x08,
     210,   17,   17,   17, 0x08,
     229,   17,   17,   17, 0x08,
     248,   17,   17,   17, 0x08,
     271,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AsynchronousGrab[] = {
    "AsynchronousGrab\0\0OnBnClickedButtonStartstop()\0"
    "status\0OnFrameReady(int)\0reason\0"
    "OnCameraListChanged(int)\0"
    "OnBtnSavedImageClicked()\0"
    "OnBtnPistonCategoryClicked()\0"
    "OnBtn_1PistonLocationClicked()\0"
    "OnBtnListenClicked()\0OnBtnSendClicked()\0"
    "GetConnectStatus()\0GetMessageFromClient()\0"
    "TransformRobot()\0"
};

void AsynchronousGrab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AsynchronousGrab *_t = static_cast<AsynchronousGrab *>(_o);
        switch (_id) {
        case 0: _t->OnBnClickedButtonStartstop(); break;
        case 1: _t->OnFrameReady((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnCameraListChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->OnBtnSavedImageClicked(); break;
        case 4: _t->OnBtnPistonCategoryClicked(); break;
        case 5: _t->OnBtn_1PistonLocationClicked(); break;
        case 6: _t->OnBtnListenClicked(); break;
        case 7: _t->OnBtnSendClicked(); break;
        case 8: _t->GetConnectStatus(); break;
        case 9: _t->GetMessageFromClient(); break;
        case 10: _t->TransformRobot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AsynchronousGrab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AsynchronousGrab::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AsynchronousGrab,
      qt_meta_data_AsynchronousGrab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AsynchronousGrab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AsynchronousGrab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AsynchronousGrab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AsynchronousGrab))
        return static_cast<void*>(const_cast< AsynchronousGrab*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int AsynchronousGrab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

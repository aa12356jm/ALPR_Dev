/****************************************************************************
** Meta object code from reading C++ file 'captureThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../captureThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'captureThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_captureThread_t {
    QByteArrayData data[9];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_captureThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_captureThread_t qt_meta_stringdata_captureThread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "captureThread"
QT_MOC_LITERAL(1, 14, 8), // "captured"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 8), // "cameraid"
QT_MOC_LITERAL(4, 33, 3), // "img"
QT_MOC_LITERAL(5, 37, 11), // "capturedStr"
QT_MOC_LITERAL(6, 49, 15), // "vector<QString>"
QT_MOC_LITERAL(7, 65, 8), // "plateStr"
QT_MOC_LITERAL(8, 74, 7) // "cv::Mat"

    },
    "captureThread\0captured\0\0cameraid\0img\0"
    "capturedStr\0vector<QString>\0plateStr\0"
    "cv::Mat"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_captureThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       1,    1,   45,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,    3,    4,
    QMetaType::Void, QMetaType::QImage,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 8,    4,

       0        // eod
};

void captureThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        captureThread *_t = static_cast<captureThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->captured((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QImage(*)>(_a[2]))); break;
        case 1: _t->captured((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->capturedStr((*reinterpret_cast< vector<QString>(*)>(_a[1]))); break;
        case 3: _t->captured((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (captureThread::*_t)(int , QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&captureThread::captured)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (captureThread::*_t)(QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&captureThread::captured)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (captureThread::*_t)(vector<QString> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&captureThread::capturedStr)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (captureThread::*_t)(cv::Mat );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&captureThread::captured)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject captureThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_captureThread.data,
      qt_meta_data_captureThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *captureThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *captureThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_captureThread.stringdata0))
        return static_cast<void*>(const_cast< captureThread*>(this));
    return QThread::qt_metacast(_clname);
}

int captureThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void captureThread::captured(int _t1, QImage _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void captureThread::captured(QImage _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void captureThread::capturedStr(vector<QString> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void captureThread::captured(cv::Mat _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

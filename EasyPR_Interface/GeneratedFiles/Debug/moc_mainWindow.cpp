/****************************************************************************
** Meta object code from reading C++ file 'mainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mainWindow_t {
    QByteArrayData data[14];
    char stringdata0[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mainWindow_t qt_meta_stringdata_mainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "mainWindow"
QT_MOC_LITERAL(1, 11, 12), // "start_Singal"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 34), // "pushButton_plate_recognize_cl..."
QT_MOC_LITERAL(4, 60, 23), // "pushButton_open_clicked"
QT_MOC_LITERAL(5, 84, 31), // "pushButton_startCapture_clicked"
QT_MOC_LITERAL(6, 116, 9), // "showImage"
QT_MOC_LITERAL(7, 126, 3), // "img"
QT_MOC_LITERAL(8, 130, 7), // "cv::Mat"
QT_MOC_LITERAL(9, 138, 12), // "showPlateStr"
QT_MOC_LITERAL(10, 151, 15), // "vector<QString>"
QT_MOC_LITERAL(11, 167, 8), // "plateStr"
QT_MOC_LITERAL(12, 176, 15), // "showResultImage"
QT_MOC_LITERAL(13, 192, 16) // "showCameraStatus"

    },
    "mainWindow\0start_Singal\0\0"
    "pushButton_plate_recognize_clicked\0"
    "pushButton_open_clicked\0"
    "pushButton_startCapture_clicked\0"
    "showImage\0img\0cv::Mat\0showPlateStr\0"
    "vector<QString>\0plateStr\0showResultImage\0"
    "showCameraStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mainWindow[] = {

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
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    1,   63,    2, 0x08 /* Private */,
       6,    1,   66,    2, 0x08 /* Private */,
       9,    1,   69,    2, 0x08 /* Private */,
      12,    1,   72,    2, 0x08 /* Private */,
      13,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,    7,
    QMetaType::Void, 0x80000000 | 8,    7,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QImage,    7,
    QMetaType::Void,

       0        // eod
};

void mainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mainWindow *_t = static_cast<mainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start_Singal(); break;
        case 1: _t->pushButton_plate_recognize_clicked(); break;
        case 2: _t->pushButton_open_clicked(); break;
        case 3: _t->pushButton_startCapture_clicked(); break;
        case 4: _t->showImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 5: _t->showImage((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 6: _t->showPlateStr((*reinterpret_cast< vector<QString>(*)>(_a[1]))); break;
        case 7: _t->showResultImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 8: _t->showCameraStatus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (mainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mainWindow::start_Singal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject mainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_mainWindow.data,
      qt_meta_data_mainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mainWindow.stringdata0))
        return static_cast<void*>(const_cast< mainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int mainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
void mainWindow::start_Singal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "on_loginBtn_clicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 18), // "on_sendBtn_clicked"
QT_MOC_LITERAL(4, 51, 18), // "on_exitBtn_clicked"
QT_MOC_LITERAL(5, 70, 11), // "onConnected"
QT_MOC_LITERAL(6, 82, 14), // "onDisconnected"
QT_MOC_LITERAL(7, 97, 17), // "onReceivedMessage"
QT_MOC_LITERAL(8, 115, 6), // "sender"
QT_MOC_LITERAL(9, 122, 4), // "text"
QT_MOC_LITERAL(10, 127, 17), // "onUserListUpdated"
QT_MOC_LITERAL(11, 145, 8), // "userList"
QT_MOC_LITERAL(12, 154, 13), // "onLoginFailed"
QT_MOC_LITERAL(13, 168, 6), // "reason"
QT_MOC_LITERAL(14, 175, 13), // "onSocketError"
QT_MOC_LITERAL(15, 189, 3) // "err"

    },
    "MainWindow\0on_loginBtn_clicked\0\0"
    "on_sendBtn_clicked\0on_exitBtn_clicked\0"
    "onConnected\0onDisconnected\0onReceivedMessage\0"
    "sender\0text\0onUserListUpdated\0userList\0"
    "onLoginFailed\0reason\0onSocketError\0"
    "err"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    2,   64,    2, 0x08 /* Private */,
      10,    1,   69,    2, 0x08 /* Private */,
      12,    1,   72,    2, 0x08 /* Private */,
      14,    1,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,
    QMetaType::Void, QMetaType::QStringList,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_loginBtn_clicked(); break;
        case 1: _t->on_sendBtn_clicked(); break;
        case 2: _t->on_exitBtn_clicked(); break;
        case 3: _t->onConnected(); break;
        case 4: _t->onDisconnected(); break;
        case 5: _t->onReceivedMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->onUserListUpdated((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 7: _t->onLoginFailed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->onSocketError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

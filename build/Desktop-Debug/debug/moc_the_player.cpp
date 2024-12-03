/****************************************************************************
** Meta object code from reading C++ file 'the_player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../the_player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'the_player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ThePlayer_t {
    QByteArrayData data[9];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThePlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThePlayer_t qt_meta_stringdata_ThePlayer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ThePlayer"
QT_MOC_LITERAL(1, 10, 7), // "shuffle"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 16), // "playStateChanged"
QT_MOC_LITERAL(4, 36, 19), // "QMediaPlayer::State"
QT_MOC_LITERAL(5, 56, 2), // "ms"
QT_MOC_LITERAL(6, 59, 6), // "jumpTo"
QT_MOC_LITERAL(7, 66, 14), // "TheButtonInfo*"
QT_MOC_LITERAL(8, 81, 6) // "button"

    },
    "ThePlayer\0shuffle\0\0playStateChanged\0"
    "QMediaPlayer::State\0ms\0jumpTo\0"
    "TheButtonInfo*\0button"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThePlayer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       6,    1,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void ThePlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ThePlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->shuffle(); break;
        case 1: _t->playStateChanged((*reinterpret_cast< QMediaPlayer::State(*)>(_a[1]))); break;
        case 2: _t->jumpTo((*reinterpret_cast< TheButtonInfo*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaPlayer::State >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ThePlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QMediaPlayer::staticMetaObject>(),
    qt_meta_stringdata_ThePlayer.data,
    qt_meta_data_ThePlayer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ThePlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThePlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ThePlayer.stringdata0))
        return static_cast<void*>(this);
    return QMediaPlayer::qt_metacast(_clname);
}

int ThePlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMediaPlayer::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

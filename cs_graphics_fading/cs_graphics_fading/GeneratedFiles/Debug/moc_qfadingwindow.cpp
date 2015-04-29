/****************************************************************************
** Meta object code from reading C++ file 'qfadingwindow.h'
**
** Created: Sat Apr 25 10:05:41 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qfadingwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qfadingwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QFadingWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      26,   14,   14,   14, 0x08,
      41,   14,   14,   14, 0x08,
      57,   14,   14,   14, 0x08,
      74,   14,   14,   14, 0x08,
      90,   14,   14,   14, 0x08,
     105,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QFadingWindow[] = {
    "QFadingWindow\0\0openFile()\0normalRender()\0"
    "zBufferRender()\0scanningRender()\0"
    "sectionRender()\0setColorType()\0"
    "setRandomColor()\0"
};

const QMetaObject QFadingWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QFadingWindow,
      qt_meta_data_QFadingWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QFadingWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QFadingWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QFadingWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QFadingWindow))
        return static_cast<void*>(const_cast< QFadingWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QFadingWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openFile(); break;
        case 1: normalRender(); break;
        case 2: zBufferRender(); break;
        case 3: scanningRender(); break;
        case 4: sectionRender(); break;
        case 5: setColorType(); break;
        case 6: setRandomColor(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

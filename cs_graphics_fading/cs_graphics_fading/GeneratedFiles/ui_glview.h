/********************************************************************************
** Form generated from reading UI file 'glview.ui'
**
** Created: Sat Apr 25 10:05:42 2015
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLVIEW_H
#define UI_GLVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtOpenGL/QGLWidget>

QT_BEGIN_NAMESPACE

class Ui_GLView
{
public:

    void setupUi(QGLWidget *GLView)
    {
        if (GLView->objectName().isEmpty())
            GLView->setObjectName(QString::fromUtf8("GLView"));
        GLView->resize(400, 300);

        retranslateUi(GLView);

        QMetaObject::connectSlotsByName(GLView);
    } // setupUi

    void retranslateUi(QGLWidget *GLView)
    {
        GLView->setWindowTitle(QApplication::translate("GLView", "GLView", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GLView: public Ui_GLView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLVIEW_H

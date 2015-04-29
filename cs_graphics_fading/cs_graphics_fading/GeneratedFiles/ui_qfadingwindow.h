/********************************************************************************
** Form generated from reading UI file 'qfadingwindow.ui'
**
** Created: Sat Apr 25 10:05:42 2015
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QFADINGWINDOW_H
#define UI_QFADINGWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QFadingWindowClass
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionZbuffer;
    QAction *actionScanning_Line;
    QAction *actionSection_Zbuffer;
    QAction *actionNormal;
    QAction *actionChange_Color;
    QAction *actionChange;
    QAction *actionSet_Color;
    QWidget *centralWidget;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuFading;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QFadingWindowClass)
    {
        if (QFadingWindowClass->objectName().isEmpty())
            QFadingWindowClass->setObjectName(QString::fromUtf8("QFadingWindowClass"));
        QFadingWindowClass->resize(1126, 754);
        actionOpen = new QAction(QFadingWindowClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../Icon/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionExit = new QAction(QFadingWindowClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionZbuffer = new QAction(QFadingWindowClass);
        actionZbuffer->setObjectName(QString::fromUtf8("actionZbuffer"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../Icon/points.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZbuffer->setIcon(icon1);
        actionScanning_Line = new QAction(QFadingWindowClass);
        actionScanning_Line->setObjectName(QString::fromUtf8("actionScanning_Line"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../Icon/selected_edge.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScanning_Line->setIcon(icon2);
        actionSection_Zbuffer = new QAction(QFadingWindowClass);
        actionSection_Zbuffer->setObjectName(QString::fromUtf8("actionSection_Zbuffer"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("../Icon/cutingplane.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSection_Zbuffer->setIcon(icon3);
        actionNormal = new QAction(QFadingWindowClass);
        actionNormal->setObjectName(QString::fromUtf8("actionNormal"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("../Icon/flat.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNormal->setIcon(icon4);
        actionChange_Color = new QAction(QFadingWindowClass);
        actionChange_Color->setObjectName(QString::fromUtf8("actionChange_Color"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("../Icon/ball.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionChange_Color->setIcon(icon5);
        actionChange = new QAction(QFadingWindowClass);
        actionChange->setObjectName(QString::fromUtf8("actionChange"));
        actionSet_Color = new QAction(QFadingWindowClass);
        actionSet_Color->setObjectName(QString::fromUtf8("actionSet_Color"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("../Icon/EntityItem.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSet_Color->setIcon(icon6);
        centralWidget = new QWidget(QFadingWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setGeometry(QRect(-10, -60, 1141, 781));
        mdiArea->setViewMode(QMdiArea::TabbedView);
        mdiArea->setTabShape(QTabWidget::Triangular);
        QFadingWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QFadingWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1126, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFading = new QMenu(menuBar);
        menuFading->setObjectName(QString::fromUtf8("menuFading"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        QFadingWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QFadingWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QFadingWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QFadingWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QFadingWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuFading->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExit);
        menuFading->addAction(actionZbuffer);
        menuFading->addAction(actionScanning_Line);
        menuFading->addAction(actionSection_Zbuffer);
        menuFading->addAction(actionNormal);
        menuEdit->addAction(actionChange_Color);
        menuEdit->addAction(actionSet_Color);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionNormal);
        mainToolBar->addAction(actionZbuffer);
        mainToolBar->addAction(actionScanning_Line);
        mainToolBar->addAction(actionSection_Zbuffer);
        mainToolBar->addAction(actionChange_Color);
        mainToolBar->addAction(actionSet_Color);

        retranslateUi(QFadingWindowClass);

        QMetaObject::connectSlotsByName(QFadingWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *QFadingWindowClass)
    {
        QFadingWindowClass->setWindowTitle(QApplication::translate("QFadingWindowClass", "QFadingWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("QFadingWindowClass", "Open", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("QFadingWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionZbuffer->setText(QApplication::translate("QFadingWindowClass", "Zbuffer", 0, QApplication::UnicodeUTF8));
        actionScanning_Line->setText(QApplication::translate("QFadingWindowClass", "Scanning Line", 0, QApplication::UnicodeUTF8));
        actionSection_Zbuffer->setText(QApplication::translate("QFadingWindowClass", "Section Zbuffer", 0, QApplication::UnicodeUTF8));
        actionNormal->setText(QApplication::translate("QFadingWindowClass", "Normal", 0, QApplication::UnicodeUTF8));
        actionChange_Color->setText(QApplication::translate("QFadingWindowClass", "Single Color", 0, QApplication::UnicodeUTF8));
        actionChange->setText(QApplication::translate("QFadingWindowClass", "change", 0, QApplication::UnicodeUTF8));
        actionSet_Color->setText(QApplication::translate("QFadingWindowClass", "Random Color", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("QFadingWindowClass", "File", 0, QApplication::UnicodeUTF8));
        menuFading->setTitle(QApplication::translate("QFadingWindowClass", "Fading", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("QFadingWindowClass", "Edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QFadingWindowClass: public Ui_QFadingWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QFADINGWINDOW_H

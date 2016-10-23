/********************************************************************************
** Form generated from reading UI file 'meshviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESHVIEWER_H
#define UI_MESHVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeshViewerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MeshViewerClass)
    {
        if (MeshViewerClass->objectName().isEmpty())
            MeshViewerClass->setObjectName(QStringLiteral("MeshViewerClass"));
        MeshViewerClass->resize(600, 400);
        menuBar = new QMenuBar(MeshViewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        MeshViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MeshViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MeshViewerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MeshViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MeshViewerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MeshViewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MeshViewerClass->setStatusBar(statusBar);

        retranslateUi(MeshViewerClass);

        QMetaObject::connectSlotsByName(MeshViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *MeshViewerClass)
    {
        MeshViewerClass->setWindowTitle(QApplication::translate("MeshViewerClass", "MeshViewer", 0));
    } // retranslateUi

};

namespace Ui {
    class MeshViewerClass: public Ui_MeshViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESHVIEWER_H

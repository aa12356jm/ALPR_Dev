/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QAction *actionopen;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QLabel *label_3;
    QGroupBox *groupBox_4;
    QLabel *label_2;
    QGraphicsView *graphicsView_camera;
    QTableView *tableView_results;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName(QStringLiteral("mainWindowClass"));
        mainWindowClass->resize(1072, 747);
        actionopen = new QAction(mainWindowClass);
        actionopen->setObjectName(QStringLiteral("actionopen"));
        centralWidget = new QWidget(mainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(510, 30, 511, 131));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 30, 451, 71));
        label_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(0, 10, 481, 671));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 370, 301, 261));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        graphicsView_camera = new QGraphicsView(groupBox_4);
        graphicsView_camera->setObjectName(QStringLiteral("graphicsView_camera"));
        graphicsView_camera->setGeometry(QRect(60, 40, 391, 251));
        tableView_results = new QTableView(centralWidget);
        tableView_results->setObjectName(QStringLiteral("tableView_results"));
        tableView_results->setGeometry(QRect(530, 410, 301, 151));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(540, 180, 371, 192));
        mainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1072, 23));
        mainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mainWindowClass->setStatusBar(statusBar);

        retranslateUi(mainWindowClass);

        QMetaObject::connectSlotsByName(mainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindowClass)
    {
        mainWindowClass->setWindowTitle(QApplication::translate("mainWindowClass", "mainWindow", Q_NULLPTR));
        actionopen->setText(QApplication::translate("mainWindowClass", "open", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\345\214\272\345\237\237", Q_NULLPTR));
        label_3->setText(QString());
        groupBox_4->setTitle(QApplication::translate("mainWindowClass", "GroupBox", Q_NULLPTR));
        label_2->setText(QApplication::translate("mainWindowClass", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QAction *actionopen;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_plate_locate;
    QPushButton *pushButton_plate_judge;
    QPushButton *pushButton_plate_detect;
    QPushButton *pushButton_chars_segment;
    QPushButton *pushButton_chars_identify;
    QPushButton *pushButton_chars_recognise;
    QPushButton *pushButton_plate_recognize;
    QPushButton *pushButton_test_all;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QPushButton *pushButton_11;
    QPushButton *pushButton_10;
    QPushButton *pushButton_9;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName(QStringLiteral("mainWindowClass"));
        mainWindowClass->resize(912, 532);
        actionopen = new QAction(mainWindowClass);
        actionopen->setObjectName(QStringLiteral("actionopen"));
        centralWidget = new QWidget(mainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 641, 401));
        groupBox->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(680, 30, 101, 391));
        groupBox_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton_plate_locate = new QPushButton(groupBox_2);
        pushButton_plate_locate->setObjectName(QStringLiteral("pushButton_plate_locate"));

        gridLayout_2->addWidget(pushButton_plate_locate, 0, 0, 1, 1);

        pushButton_plate_judge = new QPushButton(groupBox_2);
        pushButton_plate_judge->setObjectName(QStringLiteral("pushButton_plate_judge"));

        gridLayout_2->addWidget(pushButton_plate_judge, 1, 0, 1, 1);

        pushButton_plate_detect = new QPushButton(groupBox_2);
        pushButton_plate_detect->setObjectName(QStringLiteral("pushButton_plate_detect"));

        gridLayout_2->addWidget(pushButton_plate_detect, 2, 0, 1, 1);

        pushButton_chars_segment = new QPushButton(groupBox_2);
        pushButton_chars_segment->setObjectName(QStringLiteral("pushButton_chars_segment"));

        gridLayout_2->addWidget(pushButton_chars_segment, 3, 0, 1, 1);

        pushButton_chars_identify = new QPushButton(groupBox_2);
        pushButton_chars_identify->setObjectName(QStringLiteral("pushButton_chars_identify"));

        gridLayout_2->addWidget(pushButton_chars_identify, 4, 0, 1, 1);

        pushButton_chars_recognise = new QPushButton(groupBox_2);
        pushButton_chars_recognise->setObjectName(QStringLiteral("pushButton_chars_recognise"));

        gridLayout_2->addWidget(pushButton_chars_recognise, 5, 0, 1, 1);

        pushButton_plate_recognize = new QPushButton(groupBox_2);
        pushButton_plate_recognize->setObjectName(QStringLiteral("pushButton_plate_recognize"));

        gridLayout_2->addWidget(pushButton_plate_recognize, 6, 0, 1, 1);

        pushButton_test_all = new QPushButton(groupBox_2);
        pushButton_test_all->setObjectName(QStringLiteral("pushButton_test_all"));

        gridLayout_2->addWidget(pushButton_test_all, 7, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(790, 30, 101, 391));
        groupBox_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        pushButton_11 = new QPushButton(groupBox_3);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));

        gridLayout_3->addWidget(pushButton_11, 2, 0, 1, 1);

        pushButton_10 = new QPushButton(groupBox_3);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));

        gridLayout_3->addWidget(pushButton_10, 1, 0, 1, 1);

        pushButton_9 = new QPushButton(groupBox_3);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        gridLayout_3->addWidget(pushButton_9, 0, 0, 1, 1);

        pushButton_12 = new QPushButton(groupBox_3);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));

        gridLayout_3->addWidget(pushButton_12, 3, 0, 1, 1);

        pushButton_13 = new QPushButton(groupBox_3);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));

        gridLayout_3->addWidget(pushButton_13, 4, 0, 1, 1);

        mainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 912, 23));
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
        groupBox->setTitle(QApplication::translate("mainWindowClass", "\346\230\276\347\244\272", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("mainWindowClass", "\346\265\213\350\257\225", Q_NULLPTR));
        pushButton_plate_locate->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\345\256\232\344\275\215", Q_NULLPTR));
        pushButton_plate_judge->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\345\210\244\346\226\255", Q_NULLPTR));
        pushButton_plate_detect->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\346\243\200\346\265\213", Q_NULLPTR));
        pushButton_chars_segment->setText(QApplication::translate("mainWindowClass", "\345\255\227\347\254\246\345\210\206\345\211\262", Q_NULLPTR));
        pushButton_chars_identify->setText(QApplication::translate("mainWindowClass", "\345\255\227\347\254\246\351\211\264\345\210\253", Q_NULLPTR));
        pushButton_chars_recognise->setText(QApplication::translate("mainWindowClass", "\345\255\227\347\254\246\350\257\206\345\210\253", Q_NULLPTR));
        pushButton_plate_recognize->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\350\257\206\345\210\253", Q_NULLPTR));
        pushButton_test_all->setText(QApplication::translate("mainWindowClass", "\346\265\213\350\257\225\345\205\250\351\203\250", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("mainWindowClass", "\346\211\271\351\207\217\346\265\213\350\257\225", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("mainWindowClass", "Grid_search", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("mainWindowClass", "Native_test", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("mainWindowClass", "General_test", Q_NULLPTR));
        pushButton_12->setText(QApplication::translate("mainWindowClass", "SVM\350\256\255\347\273\203", Q_NULLPTR));
        pushButton_13->setText(QApplication::translate("mainWindowClass", "ANN\350\256\255\347\273\203", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

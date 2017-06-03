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
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QAction *actionopen;
    QWidget *centralWidget;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_plate_recognize;
    QPushButton *pushButton_plate_recognize_3;
    QPushButton *pushButton_plate_recognize_2;
    QPushButton *pushButton_plate_recognize_4;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QTextBrowser *textBrowser;
    QTextBrowser *textBrowser_2;
    QTextBrowser *textBrowser_3;
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
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout_3 = new QGridLayout(groupBox_4);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        groupBox = new QGroupBox(groupBox_4);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(groupBox_4);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        groupBox_2->setMinimumSize(QSize(80, 0));
        groupBox_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton_plate_recognize = new QPushButton(groupBox_2);
        pushButton_plate_recognize->setObjectName(QStringLiteral("pushButton_plate_recognize"));

        gridLayout_2->addWidget(pushButton_plate_recognize, 0, 0, 1, 1);

        pushButton_plate_recognize_3 = new QPushButton(groupBox_2);
        pushButton_plate_recognize_3->setObjectName(QStringLiteral("pushButton_plate_recognize_3"));

        gridLayout_2->addWidget(pushButton_plate_recognize_3, 2, 0, 1, 1);

        pushButton_plate_recognize_2 = new QPushButton(groupBox_2);
        pushButton_plate_recognize_2->setObjectName(QStringLiteral("pushButton_plate_recognize_2"));

        gridLayout_2->addWidget(pushButton_plate_recognize_2, 1, 0, 1, 1);

        pushButton_plate_recognize_4 = new QPushButton(groupBox_2);
        pushButton_plate_recognize_4->setObjectName(QStringLiteral("pushButton_plate_recognize_4"));

        gridLayout_2->addWidget(pushButton_plate_recognize_4, 3, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBox_2);


        gridLayout_3->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy3);
        groupBox_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        textBrowser = new QTextBrowser(groupBox_3);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        sizePolicy2.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(textBrowser);

        textBrowser_2 = new QTextBrowser(groupBox_3);
        textBrowser_2->setObjectName(QStringLiteral("textBrowser_2"));
        sizePolicy2.setHeightForWidth(textBrowser_2->sizePolicy().hasHeightForWidth());
        textBrowser_2->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(textBrowser_2);

        textBrowser_3 = new QTextBrowser(groupBox_3);
        textBrowser_3->setObjectName(QStringLiteral("textBrowser_3"));
        sizePolicy2.setHeightForWidth(textBrowser_3->sizePolicy().hasHeightForWidth());
        textBrowser_3->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(textBrowser_3);


        gridLayout_4->addLayout(horizontalLayout_3, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox_3);


        gridLayout_5->addLayout(verticalLayout, 0, 0, 1, 1);

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
        groupBox_4->setTitle(QApplication::translate("mainWindowClass", "GroupBox", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("mainWindowClass", "\345\233\276\345\203\217\346\230\276\347\244\272", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("mainWindowClass", "\346\265\213\350\257\225", Q_NULLPTR));
        pushButton_plate_recognize->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\350\257\206\345\210\253", Q_NULLPTR));
        pushButton_plate_recognize_3->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\350\257\206\345\210\253", Q_NULLPTR));
        pushButton_plate_recognize_2->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\350\257\206\345\210\253", Q_NULLPTR));
        pushButton_plate_recognize_4->setText(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\350\257\206\345\210\253", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("mainWindowClass", "\350\276\223\345\207\272\344\277\241\346\201\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

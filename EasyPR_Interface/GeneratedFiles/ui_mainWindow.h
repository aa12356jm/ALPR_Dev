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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
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
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *graphicsView_easyPR;
    QTextBrowser *textBrowser_easyPR;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView_openALPR;
    QTextBrowser *textBrowser_openALPR;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QLabel *label_3;
    QTableView *tableView_results;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName(QStringLiteral("mainWindowClass"));
        mainWindowClass->resize(1161, 810);
        actionopen = new QAction(mainWindowClass);
        actionopen->setObjectName(QStringLiteral("actionopen"));
        centralWidget = new QWidget(mainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_4 = new QGroupBox(tab);
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
        graphicsView_easyPR = new QGraphicsView(groupBox_4);
        graphicsView_easyPR->setObjectName(QStringLiteral("graphicsView_easyPR"));

        horizontalLayout_2->addWidget(graphicsView_easyPR);

        textBrowser_easyPR = new QTextBrowser(groupBox_4);
        textBrowser_easyPR->setObjectName(QStringLiteral("textBrowser_easyPR"));
        sizePolicy.setHeightForWidth(textBrowser_easyPR->sizePolicy().hasHeightForWidth());
        textBrowser_easyPR->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(textBrowser_easyPR);


        gridLayout_3->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox_4);

        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        graphicsView_openALPR = new QGraphicsView(groupBox_2);
        graphicsView_openALPR->setObjectName(QStringLiteral("graphicsView_openALPR"));

        horizontalLayout->addWidget(graphicsView_openALPR);

        textBrowser_openALPR = new QTextBrowser(groupBox_2);
        textBrowser_openALPR->setObjectName(QStringLiteral("textBrowser_openALPR"));
        sizePolicy.setHeightForWidth(textBrowser_openALPR->sizePolicy().hasHeightForWidth());
        textBrowser_openALPR->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(textBrowser_openALPR);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);


        gridLayout_4->addLayout(verticalLayout, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(80, 110, 511, 131));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 30, 451, 71));
        label_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        tableView_results = new QTableView(tab_2);
        tableView_results->setObjectName(QStringLiteral("tableView_results"));
        tableView_results->setGeometry(QRect(270, 350, 511, 151));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        mainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1161, 23));
        mainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mainWindowClass->setStatusBar(statusBar);

        retranslateUi(mainWindowClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindowClass)
    {
        mainWindowClass->setWindowTitle(QApplication::translate("mainWindowClass", "mainWindow", Q_NULLPTR));
        actionopen->setText(QApplication::translate("mainWindowClass", "open", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("mainWindowClass", "easyPR", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("mainWindowClass", "openALPR", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainWindowClass", "Tab 1", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("mainWindowClass", "\350\275\246\347\211\214\345\214\272\345\237\237", Q_NULLPTR));
        label_3->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("mainWindowClass", "Tab 2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

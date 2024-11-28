/********************************************************************************
** Form generated from reading UI file 'inputdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTDLG_H
#define UI_INPUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_inputdlg
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QDialog *inputdlg)
    {
        if (inputdlg->objectName().isEmpty())
            inputdlg->setObjectName(QString::fromUtf8("inputdlg"));
        inputdlg->resize(600, 300);
        gridLayout = new QGridLayout(inputdlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 6);
        scrollArea = new QScrollArea(inputdlg);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 575, 288));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(inputdlg);

        QMetaObject::connectSlotsByName(inputdlg);
    } // setupUi

    void retranslateUi(QDialog *inputdlg)
    {
        inputdlg->setWindowTitle(QApplication::translate("inputdlg", "Input Sliders", nullptr));
    } // retranslateUi

};

namespace Ui {
    class inputdlg: public Ui_inputdlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTDLG_H

/********************************************************************************
** Form generated from reading UI file 'outputdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUTDLG_H
#define UI_OUTPUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OutputDlg
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QDialog *OutputDlg)
    {
        if (OutputDlg->objectName().isEmpty())
            OutputDlg->setObjectName(QString::fromUtf8("OutputDlg"));
        OutputDlg->resize(600, 300);
        gridLayout = new QGridLayout(OutputDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 6);
        scrollArea = new QScrollArea(OutputDlg);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 571, 288));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(OutputDlg);

        QMetaObject::connectSlotsByName(OutputDlg);
    } // setupUi

    void retranslateUi(QDialog *OutputDlg)
    {
        OutputDlg->setWindowTitle(QApplication::translate("OutputDlg", "Output View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OutputDlg: public Ui_OutputDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUTDLG_H

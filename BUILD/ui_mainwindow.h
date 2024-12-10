/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout_3;
    QPushButton *pbShowInput;
    QPushButton *pbEnable;
    QPushButton *pbShowOutput;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QLineEdit *edtPortOp;
    QLabel *label_2;
    QLabel *label;
    QLabel *lblTc;
    QLabel *lblInfo;
    QLabel *lblFps;
    QLabel *lblSTc;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *edtAddress;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *edtPort;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *edtSendTo;
    QCheckBox *cbSineWave;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(301, 260);
        MainWindow->setMinimumSize(QSize(300, 0));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pbShowInput = new QPushButton(centralWidget);
        pbShowInput->setObjectName(QString::fromUtf8("pbShowInput"));

        gridLayout_3->addWidget(pbShowInput, 2, 0, 1, 1);

        pbEnable = new QPushButton(centralWidget);
        pbEnable->setObjectName(QString::fromUtf8("pbEnable"));

        gridLayout_3->addWidget(pbEnable, 0, 0, 1, 1);

        pbShowOutput = new QPushButton(centralWidget);
        pbShowOutput->setObjectName(QString::fromUtf8("pbShowOutput"));

        gridLayout_3->addWidget(pbShowOutput, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_3, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_4->addWidget(label_6);

        edtPortOp = new QLineEdit(centralWidget);
        edtPortOp->setObjectName(QString::fromUtf8("edtPortOp"));
        edtPortOp->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(edtPortOp);


        gridLayout->addLayout(horizontalLayout_4, 6, 2, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 2, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        lblTc = new QLabel(centralWidget);
        lblTc->setObjectName(QString::fromUtf8("lblTc"));

        gridLayout->addWidget(lblTc, 3, 2, 1, 1);

        lblInfo = new QLabel(centralWidget);
        lblInfo->setObjectName(QString::fromUtf8("lblInfo"));

        gridLayout->addWidget(lblInfo, 2, 2, 1, 1);

        lblFps = new QLabel(centralWidget);
        lblFps->setObjectName(QString::fromUtf8("lblFps"));

        gridLayout->addWidget(lblFps, 2, 0, 1, 1);

        lblSTc = new QLabel(centralWidget);
        lblSTc->setObjectName(QString::fromUtf8("lblSTc"));

        gridLayout->addWidget(lblSTc, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        edtAddress = new QLineEdit(centralWidget);
        edtAddress->setObjectName(QString::fromUtf8("edtAddress"));
        edtAddress->setEnabled(false);
        edtAddress->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        edtAddress->setReadOnly(true);

        horizontalLayout->addWidget(edtAddress);


        gridLayout->addLayout(horizontalLayout, 5, 0, 1, 1);

        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 1, 6, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);

        edtPort = new QLineEdit(centralWidget);
        edtPort->setObjectName(QString::fromUtf8("edtPort"));
        edtPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(edtPort);


        gridLayout->addLayout(horizontalLayout_2, 6, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        edtSendTo = new QLineEdit(centralWidget);
        edtSendTo->setObjectName(QString::fromUtf8("edtSendTo"));
        edtSendTo->setEnabled(false);
        edtSendTo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        edtSendTo->setReadOnly(true);

        horizontalLayout_5->addWidget(edtSendTo);


        gridLayout->addLayout(horizontalLayout_5, 5, 2, 1, 1);

        cbSineWave = new QCheckBox(centralWidget);
        cbSineWave->setObjectName(QString::fromUtf8("cbSineWave"));

        gridLayout->addWidget(cbSineWave, 4, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 301, 21));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pbShowInput->setText(QApplication::translate("MainWindow", "Show Input Window", nullptr));
        pbEnable->setText(QApplication::translate("MainWindow", "Enable Port", nullptr));
        pbShowOutput->setText(QApplication::translate("MainWindow", "Show Output Window", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Port", nullptr));
        edtPortOp->setInputMask(QApplication::translate("MainWindow", "#####", nullptr));
        edtPortOp->setText(QApplication::translate("MainWindow", "12345", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "UDP Output", nullptr));
        label->setText(QApplication::translate("MainWindow", "UDP Input", nullptr));
        lblTc->setText(QApplication::translate("MainWindow", "Output TC: ", nullptr));
        lblInfo->setText(QApplication::translate("MainWindow", "Output Info:", nullptr));
        lblFps->setText(QApplication::translate("MainWindow", "FPS:", nullptr));
        lblSTc->setText(QApplication::translate("MainWindow", "Sync TC: ", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Receive From", nullptr));
        edtAddress->setInputMask(QApplication::translate("MainWindow", "###.###.###.###", nullptr));
        edtAddress->setText(QApplication::translate("MainWindow", "0.0.0.0", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Port", nullptr));
        edtPort->setInputMask(QApplication::translate("MainWindow", "#####", nullptr));
        edtPort->setText(QApplication::translate("MainWindow", "12345", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Sender", nullptr));
        edtSendTo->setInputMask(QApplication::translate("MainWindow", "###.###.###.###", nullptr));
        edtSendTo->setText(QApplication::translate("MainWindow", "0.0.0.0", nullptr));
        cbSineWave->setText(QApplication::translate("MainWindow", "Send Sine Wave", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

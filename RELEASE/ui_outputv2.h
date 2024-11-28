/********************************************************************************
** Form generated from reading UI file 'outputv2.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUTV2_H
#define UI_OUTPUTV2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_outputV2
{
public:
    QGridLayout *gridLayout;
    QFrame *frameBorder;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFrame *frameSlider;
    QProgressBar *pbLow;
    QProgressBar *pbHigh;
    QSlider *sliderOutput;

    void setupUi(QWidget *outputV2)
    {
        if (outputV2->objectName().isEmpty())
            outputV2->setObjectName(QString::fromUtf8("outputV2"));
        outputV2->resize(350, 188);
        gridLayout = new QGridLayout(outputV2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        frameBorder = new QFrame(outputV2);
        frameBorder->setObjectName(QString::fromUtf8("frameBorder"));
        frameBorder->setFrameShape(QFrame::NoFrame);
        frameBorder->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frameBorder);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 1, 2, 2);
        label = new QLabel(frameBorder);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);
        label->setMargin(0);

        verticalLayout->addWidget(label);

        frameSlider = new QFrame(frameBorder);
        frameSlider->setObjectName(QString::fromUtf8("frameSlider"));
        frameSlider->setFrameShape(QFrame::NoFrame);
        frameSlider->setFrameShadow(QFrame::Raised);
        frameSlider->setLineWidth(0);
        pbLow = new QProgressBar(frameSlider);
        pbLow->setObjectName(QString::fromUtf8("pbLow"));
        pbLow->setGeometry(QRect(20, 30, 141, 23));
        pbLow->setMaximum(1000);
        pbLow->setValue(500);
        pbLow->setTextVisible(false);
        pbLow->setInvertedAppearance(true);
        pbLow->setTextDirection(QProgressBar::TopToBottom);
        pbHigh = new QProgressBar(frameSlider);
        pbHigh->setObjectName(QString::fromUtf8("pbHigh"));
        pbHigh->setGeometry(QRect(167, 30, 131, 23));
        pbHigh->setMaximum(1000);
        pbHigh->setValue(500);
        pbHigh->setTextVisible(false);
        sliderOutput = new QSlider(frameSlider);
        sliderOutput->setObjectName(QString::fromUtf8("sliderOutput"));
        sliderOutput->setGeometry(QRect(30, 60, 261, 20));
        sliderOutput->setMinimum(-1000);
        sliderOutput->setMaximum(1000);
        sliderOutput->setOrientation(Qt::Horizontal);
        sliderOutput->setInvertedAppearance(false);
        sliderOutput->setInvertedControls(false);
        pbHigh->raise();
        pbLow->raise();
        sliderOutput->raise();

        verticalLayout->addWidget(frameSlider);


        gridLayout->addWidget(frameBorder, 0, 0, 1, 1);


        retranslateUi(outputV2);

        QMetaObject::connectSlotsByName(outputV2);
    } // setupUi

    void retranslateUi(QWidget *outputV2)
    {
        outputV2->setWindowTitle(QApplication::translate("outputV2", "Form", nullptr));
        label->setText(QApplication::translate("outputV2", "TextLabel", nullptr));
        pbLow->setFormat(QString());
        pbHigh->setFormat(QString());
    } // retranslateUi

};

namespace Ui {
    class outputV2: public Ui_outputV2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUTV2_H

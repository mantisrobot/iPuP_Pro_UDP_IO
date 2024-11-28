/********************************************************************************
** Form generated from reading UI file 'output.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUT_H
#define UI_OUTPUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_output
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSlider *slider;

    void setupUi(QWidget *output)
    {
        if (output->objectName().isEmpty())
            output->setObjectName(QString::fromUtf8("output"));
        output->resize(150, 37);
        output->setMinimumSize(QSize(100, 0));
        output->setMaximumSize(QSize(150, 37));
        gridLayout = new QGridLayout(output);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(output);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Plain);
        frame->setMidLineWidth(0);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(true);
        label->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setPointSize(8);
        label->setFont(font);
        label->setFrameShape(QFrame::NoFrame);
        label->setTextFormat(Qt::PlainText);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(false);
        label->setMargin(1);
        label->setIndent(-1);

        verticalLayout->addWidget(label);

        slider = new QSlider(frame);
        slider->setObjectName(QString::fromUtf8("slider"));
        slider->setEnabled(true);
        slider->setMinimum(0);
        slider->setMaximum(1000);
        slider->setOrientation(Qt::Horizontal);
        slider->setTickPosition(QSlider::NoTicks);

        verticalLayout->addWidget(slider);


        gridLayout->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(output);

        QMetaObject::connectSlotsByName(output);
    } // setupUi

    void retranslateUi(QWidget *output)
    {
        output->setWindowTitle(QApplication::translate("output", "Form", nullptr));
        label->setText(QApplication::translate("output", "name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class output: public Ui_output {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUT_H

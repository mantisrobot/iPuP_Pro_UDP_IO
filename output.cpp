/*
 * output slider widget
 */

#include "output.h"
#include "ui_output.h"
#include <QDebug>

output::output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::output)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    /*
    "QSliderText::handle:horizontal {"
                            "margin-top: -2px; margin-bottom: -2px; width: 11;}"
                            "QSliderText::groove:horizontal {"
                            "height: 10px; border-radius: 4px;}"
                            */
    ui->frame->setStyleSheet("QFrame {"
                             "border: 1px solid #888;"
                             "border-radius: 4px;"
                             "padding: 0px;"
                             "}"
                             );


    ui->slider->setStyleSheet("QSlider::handle:horizontal {"
                              "width: 8px;"
                              "height: 8px;"
                              "margin: -24px -12px;"
                              "}"
                             );
/*
    ui->slider->setStyleSheet("QSlider::handle:horizontal {"
                              "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc);"
                              "border: 1px solid #777;"
                              "width: 12px;"
                              "margin-top: -8px;"
                              "margin-bottom: -8px;"
                              "margin-left: 0px;"
                              "margin-right: 0px;"
                              "border-radius: 4px;"
                              "padding: 0px;"
                              "}"
                             );
    */
    ui->label->setStyleSheet("QLabel {"
                             "border: 0px solid #888;"
                             "border-radius: 4px;"
                             "padding: 0px;"
                             "margin: 0px;"
                             "qproperty-alignment: 'AlignCenter';}"
                             );
  //  ui->slider->set



  //  this->setMaximumWidth(this->minimumWidth());
  //  this->setMaximumHeight(this->minimumHeight());

    ui->label->setFixedWidth(this->minimumWidth());
    ui->label->setFixedHeight(18);
    ui->slider->setValue(0);
}

output::~output()
{
    delete ui;
}

void output::setValue( float pos)
{
    if( pos < 0 ) ui->slider->setVisible(false);
    else
    {
        ui->slider->setVisible(true);
        ui->slider->setValue(pos*ui->slider->maximum());
    }
}

int output::value()
{
    return ui->slider->value();
}

void output::setText(char *text)
{
    ui->label->setText(text);
    if( ui->label->width() != this->width() )
    {
        ui->label->setFixedWidth(this->width());
        //qDebug() << "set width" << this->width();
    }
}

void output::setText(QString text)
{
    ui->label->setText(text);
    if( ui->label->width() != this->width() )
    {
        ui->label->setFixedWidth(this->width());
        //qDebug() << "set width" << this->width();
    }

}

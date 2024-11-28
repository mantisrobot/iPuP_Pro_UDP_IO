#include "outputv2.h"
#include "ui_outputv2.h"

outputV2::outputV2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputV2)
{
    ui->setupUi(this);

    index = -1;

    ui->frameSlider->setMinimumHeight(20);
    ui->label->setMinimumHeight(20);

    setSliderHandleVisible(false);

    ui->pbLow->setValue(0);
    ui->pbHigh->setValue(0);

    QFile qss(":/stylesheet/outputslider");
    if( qss.open(QFile::ReadOnly) == true )
    {
        QString style = qss.readAll();
        qss.close();
        ui->frameBorder->setStyleSheet(style);
        ui->label->setStyleSheet(style);
        //ui->frame->setStyleSheet(style);
        ui->pbHigh->setStyleSheet(style);
        ui->pbLow->setStyleSheet(style);
        ui->sliderOutput->setStyleSheet(style);
    }

    connect(ui->sliderOutput,SIGNAL(valueChanged(int)),this,SLOT(sliderMoved()));
}

outputV2::~outputV2()
{
    delete ui;
}

void outputV2::showEvent(QShowEvent *)
{
    resizeSliders();
}

void outputV2::resizeEvent(QResizeEvent *)
{
    resizeSliders();
}

void    outputV2::resizeSliders(void)
{
    //ui->frame->setGeometry(0,0,ui->frameSlider->width(),ui->frameSlider->height());
    ui->sliderOutput->setGeometry(0,0,ui->frameSlider->width(),ui->frameSlider->height());
    ui->pbLow->setGeometry(0,0,ui->frameSlider->width()/2,ui->frameSlider->height());
    ui->pbHigh->setGeometry((ui->frameSlider->width()/2),0,(ui->frameSlider->width()/2),ui->frameSlider->height());
}

void outputV2::setGrooveValue( float pos)
{
    ui->sliderOutput->setValue(pos*ui->sliderOutput->maximum());
    if( pos > 0.0 )
    {
        ui->pbHigh->setValue(pos*ui->pbHigh->maximum());
        ui->pbLow->setValue(0);
    }
    else if( pos < 0.0 )
    {
        ui->pbLow->setValue(-pos*ui->pbHigh->maximum());
        ui->pbHigh->setValue(0);
    }
    else
    {
        ui->pbLow->setValue(0);
        ui->pbHigh->setValue(0);
    }
}

void outputV2::setText(char *text)
{
    ui->label->setText(text);
}

void outputV2::setText(QString text)
{
    ui->label->setText(text);
}

void outputV2::setGrooveValueVisible(bool v)
{
    ui->pbHigh->setVisible(v);
    ui->pbLow->setVisible(v);
}

void outputV2::setSliderHandleVisible(bool v)
{
    ui->sliderOutput->setVisible(v);
}

void outputV2::setSliderHandleValue(float pos)
{
    ui->sliderOutput->setValue(pos*ui->sliderOutput->maximum());
}

float outputV2::getSliderHandleValue()
{
   return((float)ui->sliderOutput->value() / (float)ui->sliderOutput->maximum());
}

void outputV2::setIndex(int i)
{
    index = i;
}

void outputV2::sliderMoved()
{
    emit sliderHandleMoved(index);
}

#include "outputdlg.h"
#include "ui_outputdlg.h"

OutputDlg::OutputDlg(QWidget *parent, QString title) :
    QDialog(parent),
    ui(new Ui::OutputDlg)
{
    ui->setupUi(this);

    this->setWindowTitle(title);

    for( int t = 0; t < XP2_MAX_OUTPUTS; t++ )
    {
        oSlider[t] = new output();
        oSlider[t]->setText("Output " + QString::number(t));
        oSlider[t]->setVisible(false);
        oSlider[t]->setEnabled(true);
    }

/*
    QGridLayout *foo = new QGridLayout();
    ui->scrollAreaWidgetContents->setLayout(foo);

    int row=0,col=0;
    for( int t = 0; t < XP2_MAX_OUTPUTS; t++ )
    {
#ifdef USE_QSLIDER
        oSlider[t] = new QSlider(ui->scrollAreaWidgetContents);
        oSlider[t]->setOrientation(Qt::Horizontal);
        oSlider[t]->setMinimum(0);
        oSlider[t]->setMaximum(1000);
        oSlider[t]->setEnabled(false);
#elif defined(USE_OUTPUT_BIPOLAR)
        //  output slider bipolaar
        oSlider[t] = new outputV2(ui->scrollAreaWidgetContents);
        oSlider[t]->setText("Output " + QString::number(t));
#elif defined(USE_OUTPUT_UNIPOLAR)
        oSlider[t] = new output(ui->scrollAreaWidgetContents);
        oSlider[t]->setText("Output " + QString::number(t));
#endif
        oSlider[t]->setVisible(false);

        //


        foo->addWidget(oSlider[t],t%(XP2_MAX_OUTPUTS/4),t/(XP2_MAX_OUTPUTS/4),0);

//        oSlider[t]->move(10,t*22);
    }


    oSlider[0]->setVisible(true);
*/
    outputCount = 0;
//    ui->scrollAreaWidgetContents->setMinimumSize(0,1000);

    QGridLayout *layout = new QGridLayout();
    ui->scrollAreaWidgetContents->setLayout(layout);

}

OutputDlg::~OutputDlg()
{
    delete ui;
}

void OutputDlg::updateLayout()
{
    qDebug() << "updateLayout()";

    QGridLayout *layout = (QGridLayout*)ui->scrollAreaWidgetContents->layout();

    if ( layout != NULL )
    {
        QLayoutItem* item;
        while ( ( item = layout->takeAt( 0 ) ) != NULL )
        {
            layout->removeItem(item);
            item->widget()->setVisible(false);
//            delete item->widget();
//            delete item;
        }
        //delete ui->scrollAreaWidgetContents->layout();
    }

    if( outputCount == 0 )
    {
        return;
    }

    for( int t = 0; t < outputCount; t++ )
    {

        layout->addWidget(oSlider[t],t%(outputCount/4),t/(outputCount/4),0);
        oSlider[t]->setVisible(true);

    }

    for( int t = outputCount; t < XP2_MAX_OUTPUTS; t++ )
    {
        oSlider[t]->setVisible(false);
    }
}

void OutputDlg::updateSliders(XP2_FLOAT *ptr, int count)
{
    if( ptr == NULL ) return;

    //qDebug() << "updateSliders()" << count;

    if( count > XP2_MAX_OUTPUTS )
    {
        count = XP2_MAX_OUTPUTS;
        qDebug() << "Output count truncated!" << count << " > " << XP2_MAX_OUTPUTS;
    }

    if( count != outputCount )
    {
        qDebug() << "Layout Changed" << count;
        outputCount = count;
        updateLayout();
    }

    if( count == 0 ) return;

    for( int t = 0; t < count; t++, ptr++ )
    {
     //   oSlider[t]->setValue(*ptr * oSlider[t]->maximum() );
#ifdef USE_QSLIDER
        oSlider[t]->setValue(*ptr*1000);
#elif defined(USE_OUTPUT_BIPOLAR)
        oSlider[t]->setGrooveValue(*ptr);
#elif defined(USE_OUTPUT_UNIPOLAR)
        if( *ptr == 0 )
        {
            oSlider[t]->setValue(-1);
            oSlider[t]->setText(QString::number(t)+" Not Connected");
        }
        else
        {
            oSlider[t]->setValue(*ptr);
            oSlider[t]->setText("Output "+QString::number(t)+"   "+QString::number((*ptr-0.5)*360,'f',2));
        }
#endif
    }

}

void    OutputDlg::updateSliderName(int index, char *name)
{
    oSlider[index]->setText(name);
}

void OutputDlg::setVisibe(int index, bool visible)
{
    oSlider[index]->setVisible(visible);

    int newCount = 0;
    for( int t = 0; t < XP2_MAX_OUTPUTS; t++ )
    {
        if( oSlider[t]->isVisible() ) newCount++;
    }

    if( newCount != outputCount )
    {
        outputCount = newCount;
        this->layout()->update();
    }
}

/*
 * Input slider dialogue
 * used to set input values for the UDP input packt to iPuP-PRO
 */

#include "inputdlg.h"
#include "ui_inputdlg.h"

inputdlg::inputdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputdlg)
{
    ui->setupUi(this);

    QGridLayout *foo = new QGridLayout();
    ui->scrollAreaWidgetContents->setLayout(foo);

    //int row=0,col=0;
    for( int t = 0; t < XP2_MAX_INPUTS; t++ )
    {
        iSlider[t] = new outputV2(ui->scrollAreaWidgetContents);
        iSlider[t]->setText("Input " + QString::number(t));
        iSlider[t]->setSliderHandleVisible(true);

        foo->addWidget(iSlider[t],t%(XP2_MAX_INPUTS/4),t/(XP2_MAX_INPUTS/4),0);
        iSlider[t]->setIndex(t);

        connect(iSlider[t],SIGNAL(sliderHandleMoved(int)),this,SLOT(sliderEvent(int)));
    }
}

inputdlg::~inputdlg()
{
    delete ui;
}

void inputdlg::sliderEvent(int index)
{
    outputV2 *slider = qobject_cast<outputV2 *>(sender());
    //int index = slider->getIndex;

    emit sliderMoved(index,slider->getSliderHandleValue());
}

void inputdlg::setSliderPosition(int index,float pos)
{
    //outputV2 *slider = qobject_cast<outputV2 *>(sender());

    iSlider[index]->blockSignals(true);
    iSlider[index]->setSliderHandleValue(pos);
    iSlider[index]->blockSignals(false);
}

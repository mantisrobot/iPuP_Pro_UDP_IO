#ifndef OUTPUTDLG_H
#define OUTPUTDLG_H

#include <QDialog>
#include <QDebug>
#include <QSlider>
#include <QGridLayout>
//#include <outputv2.h>
#include <output.h>
#include <xp2defs.h>
#include <QSpacerItem>
//#include "../../../../XP2/struct/definitions.h"


//#define USE_QSLIDER
//#define USE_OUTPUT_BIPOLAR
#define USE_OUTPUT_UNIPOLAR

namespace Ui {
class OutputDlg;
}

class OutputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit OutputDlg(QWidget *parent = 0, QString title = "Output View");
    ~OutputDlg();
    void updateSliders(XP2_FLOAT *ptr, int count);
    void updateSliderName(int index, char *name);
    void setVisibe(int index, bool visible);

private:
    Ui::OutputDlg *ui;

//    QSlider *oSlider[XP2_MAX_OUTPUTS];
///    output  *oSlider[XP2_MAX_OUTPUTS];
#ifdef USE_QSLIDER
    QSlider *oSlider[XP2_MAX_OUTPUTS];
#elif defined(USE_OUTPUT_BIPOLAR)
    outputV2 *oSlder[XP2_MAX_OUTPUTS];
#elif defined(USE_OUTPUT_UNIPOLAR)
    output *oSlider[XP2_MAX_OUTPUTS];
#endif


    int outputCount = 0;
    void updateLayout();

};

#endif // OUTPUTDLG_H

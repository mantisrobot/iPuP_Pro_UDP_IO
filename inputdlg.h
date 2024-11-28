/*
 * Input slider dialogue
 * used to set input values for the UDP input packt to iPuP-PRO
 */

#ifndef INPUTDLG_H
#define INPUTDLG_H

#include <QDialog>
#include <QSlider>
#include <outputv2.h>
#include <output.h>
#include <xp2defs.h>

namespace Ui {
class inputdlg;
}

class inputdlg : public QDialog
{
    Q_OBJECT

public:
    explicit inputdlg(QWidget *parent = 0);
    ~inputdlg();

    void setSliderPosition(int,float);

private slots:
    void sliderEvent(int);

signals:
    void sliderMoved(int index,float value);

private:
    Ui::inputdlg *ui;

    outputV2 *iSlider[XP2_MAX_INPUTS];


};

#endif // INPUTDLG_H

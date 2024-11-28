/*
 * input slider widget
 */

#ifndef OUTPUTV2_H
#define OUTPUTV2_H

#include <QWidget>
#include <QFile>

namespace Ui {
class outputV2;
}

class outputV2 : public QWidget
{
    Q_OBJECT

public:
    explicit outputV2(QWidget *parent = 0);
    ~outputV2();

    void setGrooveValue( float pos);
    void setGrooveValueVisible(bool v);
    void setText(char *text);
    void setText(QString text);
    void setSliderHandleVisible( bool );
    void setSliderHandleValue( float pos);
    float getSliderHandleValue();
    void  setIndex(int);
    void  setValue(int);

signals:
    void sliderHandleMoved(int index);

private slots:
    void sliderMoved(void);

private:
    Ui::outputV2 *ui;

    void    resizeSliders(void);
    void    showEvent(QShowEvent *);
    void    resizeEvent(QResizeEvent *);

    int     index;

};

#endif // OUTPUTV2_H

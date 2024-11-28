#ifndef OUTPUT_H
#define OUTPUT_H

#include <QWidget>

namespace Ui {
class output;
}

class output : public QWidget
{
    Q_OBJECT

public:
    explicit output(QWidget *parent = 0);
    ~output();

    void setText( char * text );
    void setText( QString text );
    void setValue( float pos );
    int value();
private:
    Ui::output *ui;
};

#endif // OUTPUT_H

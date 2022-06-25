#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QEvent>
#include <QObject>
#include <QTimer>
#include <QTime>
#include "code/gui/button.h"
class Primary;

class TimeController : public QObject, public QGraphicsRectItem {
    Q_OBJECT
   public:
    TimeController(Primary *p);

    Primary *parent;

    void speed_up(int n = 1);
    void speed_down(int n = 1);
    bool is_on();
    void on(QGraphicsScene &);
    void off(QGraphicsScene &);


   private:
    bool _is_on;
    int rate;
    Button *rate_button[4];
    QTime time;

    void set_rate(int n);
    void all_button_up();
};

#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QObject>
#include <QTime>
#include <QTimer>

#include "code/gui/text.h"
class Primary;

class DigitClock : public QObject, public QGraphicsRectItem {
    Q_OBJECT
   public:
    DigitClock(Primary *p);
    ~DigitClock();

    Primary *parent;

    bool is_on();
    void on(QGraphicsScene &);
    void off(QGraphicsScene &);
    QTime current_time();
    void set_rate(int n);
    Text *text;
    void set_time(QTime time);

   signals:
    void time_tick(QTime);

   private:
    QTimer *digit_clock_timer;
    QTime time;
    bool _is_on;
    int hour = 0;
    int min = 0;
    int week = 1;
    QString weeks = "一二三四五六日";
};

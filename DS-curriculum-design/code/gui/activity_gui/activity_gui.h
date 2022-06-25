#pragma once
#include <QEvent>
#include <QGraphicsLineItem>
#include <QObject>
#include <QTimer>
#include <QVector>
#include "code/activity/event/event.h"
#include "code/gui/button.h"

class Primary;

class ActivityGUI : public QObject {
    Q_OBJECT
   public:
    ActivityGUI(Primary *);

    Primary *parent;

    int current_day;
    Button *label_date;
    Button *prev_day;
    Button *next_day;
    QGraphicsLineItem *time_line[4];
    Button *label_line[8];
    QVector<Button *> eve_activity_buttons[2000];
    Button *new_alarm[5];

   public slots:
    void update();

   private:
    QTimer *activity_gui_timer;
    void init_eve_button(Event *e);
};

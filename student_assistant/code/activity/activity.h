#pragma once
#include <QEvent>
#include <QObject>
#include <QVector>

#include "event/event.h"

class Primary;

class Activity : public QObject {
    Q_OBJECT
   public:
    Activity(Primary *);
    ~Activity();

    Primary *parent;

    QVector<Event *> eves;

    int new_alarm_time[3] = {0};

    void set_alarm(QEvent*);
    void add_event(Event *);
    void check_collision();

   public slots:
    void update(QTime);

};

#include "event.h"
#include <QDebug>
#include "code/primary.h"

Event::Event(Primary *p, QString typ, QFile &f) : Button(p) {
    parent = p;
    type = typ;
    status = pending;
    // ID is initialized in Activity::add_event()

    // type is given,
    // the following code reads:
    // 1. name
    // 2. begin_time
    // 3. end_time
    // 4. other_time
    // 5. period
    // let sub class read the rest

    // discard comments
    QString t;
    while (1) {
        t = f.readLine();
        if (t[0] != '#' && t[0] != '\n' && t[0] != '\r') break;
    }

    if (t.contains("\r\n"))
        t.chop(2);
    else
        t.chop(1);
    name = t;

    // times
    t = f.readLine();
    QStringList ls = t.split(QString(" "));
    begin_time = QTime(ls[0].toInt(), ls[1].toInt(), ls[2].toInt());

    t = f.readLine();
    ls = t.split(QString(" "));
    end_time = QTime(ls[0].toInt(), ls[1].toInt(), ls[2].toInt());

    t = f.readLine();
    ls = t.split(QString(" "));
    other_time = QTime(ls[0].toInt(), ls[1].toInt(), ls[2].toInt());

    // period
    t = f.readLine();
    int it = t.toInt();
    if (it == 0)
        period = Event::none;
    else if (it == 1)
        period = Event::week;
    else if (it == 2)
        period = Event::day;
    else if (it == 3)
        period = Event::hour;
}

Event::Event(Primary *p, EventInfo ei) : Button(p) {
    parent = p;
    type = ei.type;
    status = pending;
    name = ei.name;
    begin_time = ei.begin_time;
    end_time = ei.end_time;
    other_time = ei.other_time;
    int it = ei.period;
    if (it == 0)
        period = Event::none;
    else if (it == 1)
        period = Event::week;
    else if (it == 2)
        period = Event::day;
    else if (it == 3)
        period = Event::hour;
}

Event::~Event() {
    //  qDebug("Event deleted");
}

int Event::duration_minutes() {
    // do not cross days
    return (end_time.minute() - begin_time.minute()) * 60 +
           (end_time.second() - begin_time.second());
}

bool Event::match_begin(QTime t) { return match_time(begin_time, t, period); }
bool Event::match_end(QTime t) { return match_time(end_time, t, period); }
bool Event::match_other(QTime t) { return match_time(other_time, t, period); }

bool Event::match_time(QTime t1, QTime t2, PERIOD p) {
    int d1 = t1.hour(), h1 = t1.minute(), m1 = t1.second();
    int d2 = t2.hour(), h2 = t2.minute(), m2 = t2.second();

    if (p == week || p == none)
        ;
    else if (p == day)
        d1 = d2;
    else if (p == hour)
        d1 = d2, h1 = h2;

    // qDebug("match:%d %d %d ? %d %d %d", d1, h1, m1, d2, h2, m2);

    return QTime(d1, h1, m1) == QTime(d2, h2, m2);
}

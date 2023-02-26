#include "activity.h"

#include <QDebug>
#include <QDir>
#include <QWheelEvent>

#include "code/gui/gui.h"
#include "code/gui/residant/digit_clock.h"
#include "code/gui/residant/shell/shell.h"
#include "code/primary.h"
#include "code/tool_lib.h"
#include "event/alarm.h"
#include "event/exam.h"
#include "event/lesson.h"
#include "event/outdoors.h"

Activity::Activity(Primary *p) {
    parent = p;

    // events
    // only read cwd()/saves/events/lessons.dat & outdoors.dat & alarm.dat &
    // exams.dat,  move is not from files

    // alarms.dat
    qDebug() << cwd();
    QFile alarms_dat(cwd() + "/saves/events/alarms.dat");
    if (alarms_dat.open(QFile::ReadOnly))
        while (!alarms_dat.atEnd()) add_event(new Alarm(parent, alarms_dat));
    else
        panic("Activity: fail to open alarms.dat.");
    alarms_dat.close();

    // lessons.dat
    QFile lessons_dat(cwd() + "/saves/events/lessons.dat");
    if (lessons_dat.open(QFile::ReadOnly))
        while (!lessons_dat.atEnd()) add_event(new Lesson(parent, lessons_dat));
    else
        panic("Activity: fail to open lessons.dat.");
    lessons_dat.close();

    // exams.dat
    QFile exams_dat(cwd() + "/saves/events/exams.dat");
    if (exams_dat.open(QFile::ReadOnly))
        while (!exams_dat.atEnd()) add_event(new Exam(parent, exams_dat));
    else
        panic("Activity: fail to open exams.dat.");
    exams_dat.close();

    QFile outdoors_dat(cwd() + "/saves/events/outdoors.dat");
    if (outdoors_dat.open(QFile::ReadOnly))
        while (!outdoors_dat.atEnd())
            add_event(new Outdoors(parent, outdoors_dat));
    else
        panic("Activity: fail to open outdoors.dat");
    outdoors_dat.close();
}

Activity::~Activity() { qDebug("Activity deleted"); }

void Activity::set_alarm(QEvent *e) {
    QWheelEvent *me = (QWheelEvent *)e;
    int y = me->position().y();
    if (y >= 5 && y < 45) {
        new_alarm_time[0] += me->angleDelta().y() > 0 ? 1 : -1;
        new_alarm_time[0] += 7;
        new_alarm_time[0] %= 7;
    } else if (y < 85) {
        new_alarm_time[1] += me->angleDelta().y() > 0 ? 1 : -1;
        new_alarm_time[1] += 24;
        new_alarm_time[1] %= 24;
    } else if (y < 125) {
        new_alarm_time[2] += me->angleDelta().y() > 0 ? 1 : -1;
        new_alarm_time[2] += 60;
        new_alarm_time[2] %= 60;
    }
}

void Activity::add_event(Event *ne) {
    int ind = 0;
    for (auto i = eves.begin(); i != eves.end(); i++, ind++)
        if ((*i)->status == Event::invalid) {
            delete *i;
            ne->ID = ind;
            *i = ne;
            return;
        }

    ne->ID = ind;
    eves.push_back(ne);
}

void Activity::update(QTime t) {
    for (auto e : eves) {
        auto &sta = e->status;

        if (sta == Event::invalid) continue;

        if (sta == Event::pending && e->match_begin(t)) {
            sta = Event::use;
            e->begin_job();
            // check collition here
            check_collision();
        }

        if (sta == Event::use && e->match_end(t)) {
            e->end_job();
            if (e->period != Event::none)
                sta = Event::pending;
            else
                sta = Event::invalid;
        }

        if (e->match_other(t)) e->other_job();
    }
}

void Activity::check_collision() {
    // collision detedction
    int used[7 * 60 * 24 + 1];
    memset(used, 0, sizeof(used));
    for (auto e : eves) {
        if (e->type == "move") continue;
        int w1 = e->begin_time.hour() - 1;
        int h1 = e->begin_time.minute();
        int m1 = e->begin_time.second();

        int w2 = e->end_time.hour() - 1;
        int h2 = e->end_time.minute();
        int m2 = e->end_time.second();

        int l = (w1 * 24 + h1) * 60 + m1;
        int r = (w2 * 24 + h2) * 60 + m2;

        used[l]++;
        used[r + 1]--;
    }
    int x = 0;
    for (int i = 0; i < sizeof(used) / sizeof(int); i++) {
        x += used[i];
        if (x > 1) {
            // collision
            int w = i / (60 * 24);
            int h = (i / 60) % 24;
            int m = i % 60;
            QString week("一二三四五六日");
            QString msg = QString("[事件]于周%1%2:%3检测到冲突.")
                              .arg(week[w])
                              .arg(h)
                              .arg(m);
            parent->gui->shell->push_msg(msg, false);
            break;
        }
    }
}

#include "timetable.h"

#include <QDebug>
#include <QVector>

#include "code/activity/activity.h"
#include "code/activity/event/event.h"
#include "code/activity/event/lesson.h"
#include "code/gui/gui.h"
#include "code/gui/residant/digit_clock.h"
#include "code/gui/residant/shell/shell.h"
#include "code/primary.h"

TimeTable::TimeTable(Primary *p) { parent = p; }

TimeTable::~TimeTable() { qDebug("TimeTable deleted"); }

void TimeTable::notify(QTime t) {
    if (t.minute() != 0 || t.second() != 0) return;

    // lesson and exams of this day
    auto sl = parent->gui->shell;
    sl->push_msg("[今日课程与考试安排]", false);

    auto eves = parent->activ->eves;
    for (auto e : eves) {
        if (e->type != "lesson" && e->type != "exam") continue;
        if (e->begin_time.hour() == t.hour()) {
            QString m = e->name;
            m.prepend("-");

            QString s = QString(" %1:%2")
                            .arg(e->begin_time.minute())
                            .arg(e->begin_time.second());
            m.append(s);

            sl->push_msg(m, false);
        }
    }
}

void TimeTable::do_search(QString s) {
    search_result.clear();
    if (s == "") return;

    for (auto e : parent->activ->eves) {
        if (e->type != "lesson") continue;
        if (s.contains(e->name, Qt::CaseInsensitive) ||
            s.contains(e->lesson_building()->name(), Qt::CaseInsensitive) ||
            s.contains(e->lesson_group(), Qt::CaseInsensitive) ||
            s.contains(e->lesson_room(), Qt::CaseInsensitive) ||
            s.contains(e->lesson_teacher(), Qt::CaseInsensitive) ||
            e->name.contains(s, Qt::CaseInsensitive) ||
            e->lesson_building()->name().contains(s, Qt::CaseInsensitive) ||
            e->lesson_group().contains(s, Qt::CaseInsensitive) ||
            e->lesson_room().contains(s, Qt::CaseInsensitive) ||
            e->lesson_teacher().contains(s, Qt::CaseInsensitive))

            if (!search_result.contains(e->lesson_ID()))
                search_result.push_back(e->lesson_ID());
    }
}

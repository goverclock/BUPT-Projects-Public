#include "move.h"

#include <QDebug>

#include "code/activity/activity.h"
#include "code/gui/gui.h"
#include "code/gui/residant/digit_clock.h"
#include "code/gui/residant/shell/shell.h"
#include "code/map/building/building.h"
#include "code/map/map.h"
#include "code/primary.h"
#include "code/tool_lib.h"

Move::Move(Primary* p, EventInfo ei, QVector<Path*> paths, int n)
    : Event(p, ei) {
    parent = p;
    current_path = paths.first();
    paths.pop_front();
    solution_paths = paths;
    strategy = n;

    // if from gate to gate, then bus
    if (current_path->to_building()->name().contains("大门") &&
        current_path->from_building()->name().contains("大门")) {
        QTime ct = parent->gui->digit_clock->current_time();
        QTime et = add_minute(ct, 60);
        et.setHMS(et.hour(), et.minute(), 0);
        end_time = et;
        qDebug("%d %d %d", end_time.hour(), end_time.minute(),
               end_time.second());
    } else if (n == 0)  // time
        end_time = add_minute(begin_time, current_path->time_cost() / 60 + 1);
    else  // distance
        end_time = add_minute(begin_time, current_path->distance() / 60 + 1);

    // qDebug("t:%d d:%d", current_path->time_cost(), current_path->distance());

    // qDebug("%d %d %d to %d %d %d", begin_time.hour(), begin_time.minute(),
    //        begin_time.second(), end_time.hour(), end_time.minute(),
    //        end_time.second());
    // qDebug() << path->from_building()->name() << path->to_building()->name();
}

void Move::begin_job() {
    // qDebug("Move begin: %d %d %d", begin_time.hour(), begin_time.minute(),
    //    begin_time.second());
}

void Move::end_job() {
    // qDebug("Move end: %d %d %d", end_time.hour(), end_time.minute(),
    //    end_time.second());

    // highlight
    current_path->set_highlight(false);
    current_path->rever_path()->set_highlight(false);
    // set current_building
    parent->map->current_building = current_path->to_building()->ID();

    // shell message
    if (solution_paths.empty()) {
        QString dest = current_path->to_building()->name();
        if (dest != " ") {
            QString m = "到达";
            m.append(dest).append(".");
            parent->gui->shell->push_msg(m, true);
        }
        return;
    }

    // next move
    QTime ct = parent->gui->digit_clock->current_time();
    EventInfo ei = {"move", "移动",         add_minute(ct, 1),
                    ct,     QTime(0, 0, 0), 0};  // end time is set by Move()
    parent->activ->add_event(new Move(parent, ei, solution_paths, strategy));
}

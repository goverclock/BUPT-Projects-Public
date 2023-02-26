#include "activity_gui.h"

#include <QDebug>
#include <QString>

#include "code/activity/activity.h"
#include "code/activity/event/alarm.h"
#include "code/gui/gui.h"
#include "code/gui/residant/digit_clock.h"
#include "code/primary.h"
#include "code/tool_lib.h"
#include "code/view/view.h"

ActivityGUI::ActivityGUI(Primary* p) {
    parent = p;
    activity_gui_timer = new QTimer(this);
    activity_gui_timer->start(20);
    connect(activity_gui_timer, &QTimer::timeout, this, &ActivityGUI::update);

    // note: added to scene in GUI::all_activity_gui_on(QGraphicsScene&)
    label_date = new Button(parent, "周一", 100, 50);
    label_date->set_mode(Button::label);
    label_date->setPos(400 + 375, 0);
    label_date->setZValue(1);
    label_date->set_point_size(15);
    label_date->set_edge(false);

    // prev_button & next_button
    prev_day = new Button(parent, "<", 100, 50);
    prev_day->setPos(400 + 475, 0);
    prev_day->setZValue(1);
    prev_day->set_point_size(15);

    next_day = new Button(parent, ">", 100, 50);
    next_day->setPos(400 + 575, 0);
    next_day->setZValue(1);
    next_day->set_point_size(15);

    current_day = 1;  // invalid until update is called
    connect(prev_day, &Button::clicked, [&] {
        if (--current_day == 0) current_day = 7;
    });
    connect(next_day, &Button::clicked, [&] {
        if (++current_day == 8) current_day = 1;
    });

    // time_line & label_line
    for (int i = 0; i < 4; i++) {
        auto& tl = time_line[i];
        tl = new QGraphicsLineItem;
        QPen pen;
        pen.setWidth(3);
        pen.setColor(Qt::black);
        tl->setPen(pen);

        tl->setZValue(1);
        tl->setPos(400 + 230 * (i + 1), 100);
        tl->setLine(0, 0, 0, 720);

        auto& ll1 = label_line[i * 2];
        auto& ll2 = label_line[i * 2 + 1];
        QString t1 = QString("%1:00").arg(i * 6);
        ll1 = new Button(parent, t1, 100, 20);
        ll1->set_mode(Button::label);
        ll1->setPos(400 - 50 + 230 * (i + 1), 70);
        ll1->set_point_size(15);
        ll1->set_edge(false);

        QString t2 = QString("%1:59").arg(i * 6 + 5);
        ll2 = new Button(parent, t2, 100, 20);
        ll2->set_mode(Button::label);
        ll2->setPos(400 - 50 + 230 * (i + 1), 850);
        ll2->set_point_size(15);
        ll2->set_edge(false);
    }

    // new alarm
    auto& na = new_alarm;
    na[0] = new Button(parent, "添加闹钟>", 125, 40);
    na[0]->set_mode(Button::men);
    na[0]->setPos(400 + 675 + 125, 5);
    na[0]->setZValue(2);
    na[0]->set_point_size(10);

    na[1] = new Button(parent, "周一", 125, 40);
    na[1]->set_mode(Button::men);
    na[1]->setZValue(2);

    na[2] = new Button(parent, "0", 125, 40);
    na[2]->set_mode(Button::men);
    na[2]->setZValue(2);

    na[3] = new Button(parent, "0", 125, 40);
    na[3]->set_mode(Button::men);
    na[3]->setZValue(2);

    na[4] = new Button(parent, "确定", 125, 40);
    na[4]->set_mode(Button::men);
    na[4]->setZValue(2);

    connect(na[4], Button::clicked, [=] {
        EventInfo ei;
        ei.type = "alarm";
        ei.name = "新建闹钟";
        ei.begin_time.setHMS(parent->activ->new_alarm_time[0] + 1,
                             parent->activ->new_alarm_time[1],
                             parent->activ->new_alarm_time[2]);
        ei.end_time.setHMS(parent->activ->new_alarm_time[0] + 1,
                           parent->activ->new_alarm_time[1],
                           parent->activ->new_alarm_time[2] + 1);
        ei.period = 0;
        parent->activ->add_event(new Alarm(parent, ei));
        na[0]->menu().off();
        na[4]->up();
        parent->activ->check_collision();
    });

    na[0]->create_menu(125, 40);
    na[0]->menu().add_button(na[1]);
    na[0]->menu().add_button(na[2]);
    na[0]->menu().add_button(na[3]);
    na[0]->menu().add_button(na[4]);

    na[1]->set_point_size(10);
    na[2]->set_point_size(10);
    na[3]->set_point_size(10);
    na[4]->set_point_size(10);
}

void ActivityGUI::update() {
    // label_date also updates when switched to sce_activ
    // which is done in View::goto_scene_activ
    // note: this doesn't change current_day
    // only current_day changes, the text changes
    QString week("一二三四五六日");
    QString s = QString("周%1").arg(week[current_day - 1]);
    label_date->set_text(s);

    // eve_activity_buttons
    auto eves = parent->activ->eves;
    for (auto e : eves) {
        if (e->status == Event::invalid) {
            // delete buttons and empty the vector
            for (auto b : eve_activity_buttons[e->ID]) {
                if (b->has_menu()) b->menu().off();
                delete b;
            }
            eve_activity_buttons[e->ID].clear();
            continue;
        }

        // already initialized, refresh it
        if (!eve_activity_buttons[e->ID].empty()) {
            if (e->period == Event::none || e->period == Event::week) {
                bool visib = e->begin_time.hour() == current_day;
                for (auto i : eve_activity_buttons[e->ID]) {
                    i->setVisible(visib);
                    if (!visib && i->has_menu()) i->menu().off();
                }
            } else if (e->period == Event::day) {
                // show everyday, so do nothing
            }
            continue;
        }

        // initialize buttons for this event
        init_eve_button(e);
    }

    // new_alarm button
    int w = parent->activ->new_alarm_time[0];
    int h = parent->activ->new_alarm_time[1];
    int m = parent->activ->new_alarm_time[2];

    s = QString("周%1").arg(week[w]);
    new_alarm[1]->set_text(s);
    new_alarm[2]->set_text(QString::number(h));
    new_alarm[3]->set_text(QString::number(m));
}

void ActivityGUI::init_eve_button(Event* e) {
    if (e->period == Event::none || e->period == Event::week) {
        // single button
        int h = std::max(30, 2 * e->duration_minutes());
        auto b = new Button(parent, e->name, 100, h);
        b->set_mode(Button::label);
        b->set_point_size(10);
        b->setZValue(2);
        QTime t = e->begin_time;
        int x = 400 - 100 + (t.minute() / 6 + 1) * 230;
        int y = 100 + ((t.minute() % 6 * 60 + t.second()) * 2);
        b->setPos(x, y);
        e->create_and_init_menu(b);
        b->on(*parent->view->sce_activ);

        eve_activity_buttons[e->ID].push_back(b);
    } else if (e->period == Event::day) {
        // 7 buttons
        Button* b[7];
        for (int i = 0; i < 7; i++) {
            int h = std::max(30, 2 * e->duration_minutes());
            b[i] = new Button(parent, e->name, 100, h);
            b[i]->set_mode(Button::label);
            b[i]->set_point_size(10);
            QTime t = e->begin_time;
            int x = 400 - 100 + (t.minute() / 6 + 1) * 230;
            int y = 100 + ((t.minute() % 6 * 60 + t.second()) * 2);
            b[i]->setPos(x, y);
            e->create_and_init_menu(b[i]);
            b[i]->on(*parent->view->sce_activ);

            eve_activity_buttons[e->ID].push_back(b[i]);
        }
    } else if (e->period == Event::hour) {
        panic(
            "ActivityGUI: period == hour is not fully implemented, do not "
            "use.");
    }
}

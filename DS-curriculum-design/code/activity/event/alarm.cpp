#include "alarm.h"

#include <QDebug>
#include "code/gui/gui.h"
#include "code/gui/residant/shell/shell.h"
#include "code/primary.h"

Alarm::Alarm(Primary *p, QFile &f) : Event(p, "alarm", f) {
    parent = p;

    // no addition info
}

Alarm::Alarm(Primary *p, EventInfo ei) :Event(p, ei) {
    parent = p;

}

void Alarm::create_and_init_menu(Button *b) {
    // begin_time
    // period
    // operation > delete

    // level 1
    b->create_menu(100, 30);

    QString s1 = QString("%1:%2")
                    .arg(begin_time.minute())
                    .arg(begin_time.second());
    auto be_time = new Button(parent, s1, 100, 30);
    be_time->set_mode(Button::label);

    if(period == Event::none) s1 = "单次";
    else if(period == Event::week) s1 = "每周";
    else if(period == Event::day) s1 = "每天";
    else panic("create_and_init_menu: do not use period == hour");
    auto per = new Button(parent, s1, 100, 30);
    per->set_mode(Button::label);

    auto opr = new Button(parent, "操作>", 100, 30);
    opr->set_mode(Button::men);
    
    b->menu().add_button(be_time);
    b->menu().add_button(per);
    b->menu().add_button(opr);

    be_time->set_point_size(10);
    per->set_point_size(10);
    opr->set_point_size(10);

    // level 2
    opr->create_menu(100, 30);

    auto opr_delete = new Button(parent, "删除", 100, 30);
    opr_delete->set_mode(Button::men);

    opr->menu().add_button(opr_delete);
    opr_delete->set_point_size(10);

    connect(opr_delete, &Button::clicked, [=] {
        b->menu().off();
        status = Event::invalid;
    });
}

void Alarm::begin_job() {
    QString s = name;
    s.append("响起.");
    parent->gui->shell->push_msg(s, true);
}

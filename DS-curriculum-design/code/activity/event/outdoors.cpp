#include "outdoors.h"

#include <QDebug>

#include "code/gui/gui.h"
#include "code/gui/residant/shell/shell.h"
#include "code/map/map.h"
#include "code/primary.h"
#include "code/view/view.h"

Outdoors::Outdoors(Primary *p, QFile &f) : Event(p, "outdoors", f) {
    parent = p;

    // addition info
    // building_ID, sub_type, group
    QString t;
    while (1) {
        t = f.readLine();
        if (t[0] != '#' && t[0] != '\n' && t[0] != '\r') break;
    }

    // building_ID
    building_ID = t.toInt();

    // sub_type
    t = f.readLine();
    if (t.contains("\r\n"))
        t.chop(2);
    else
        t.chop(1);
    sub_type = t;

    // group
    t = f.readLine();
    if (t.contains("\r\n"))
        t.chop(2);
    else
        t.chop(1);
    group = t.toInt();
}

void Outdoors::create_and_init_menu(Button *b) {
    // begin_time
    // end_time
    // group
    // sub_type
    // building
    // operation > navigate

    // level 1
    b->create_menu(100, 30);

    QString s1 =
        QString("%1:%2").arg(begin_time.minute()).arg(begin_time.second());
    s1.prepend("始:");
    auto be_time = new Button(parent, s1, 100, 30);
    be_time->set_mode(Button::label);

    s1 = QString("%1:%2").arg(end_time.minute()).arg(end_time.second());
    s1.prepend("终:");
    auto ed_time = new Button(parent, s1, 100, 30);
    ed_time->set_mode(Button::label);

    if (group)
        s1 = "集体活动";
    else
        s1 = "个人活动";
    auto grp = new Button(parent, s1, 100, 30);
    grp->set_mode(Button::label);

    auto sub = new Button(parent, sub_type, 100, 30);
    sub->set_mode(Button::label);

    s1 = parent->map->buildings[building_ID]->name();
    auto pos = new Button(parent, s1, 100, 30);
    pos->set_mode(Button::label);

    auto opr = new Button(parent, "操作>", 100, 30);
    opr->set_mode(Button::men);

    b->menu().add_button(grp);
    b->menu().add_button(sub);
    b->menu().add_button(be_time);
    b->menu().add_button(ed_time);
    b->menu().add_button(pos);
    b->menu().add_button(opr);

    // level 2
    opr->create_menu(100, 30);

    auto opr_navigate = new Button(parent, "导航", 100, 30);
    opr_navigate->set_mode(Button::men);

    opr->menu().add_button(opr_navigate);

    connect(opr_navigate, &Button::clicked, [=] {
        b->menu().off();
        parent->view->goto_scene(map);
        parent->map->move_to(parent->map->buildings[building_ID], 0, 0);
    });
}

void Outdoors::begin_job() {
    QString s;
    if (group)
        s = "集体活动";
    else
        s = "个人活动";
    s.append('\"');
    s.append(sub_type);
    s.append("\"开始了.");
    parent->gui->shell->push_msg(s, true);
}

void Outdoors::end_job() {
    QString s;
    if (group)
        s = "集体活动";
    else
        s = "个人活动";
    s.append('\"');
    s.append(sub_type);
    s.append("\"结束了.");
    parent->gui->shell->push_msg(s, true);
}

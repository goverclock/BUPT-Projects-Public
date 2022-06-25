#include "exam.h"

#include <QDebug>

#include "code/gui/gui.h"
#include "code/gui/table_gui/table_gui.h"
#include "code/gui/residant/shell/shell.h"
#include "code/map/map.h"
#include "code/primary.h"
#include "code/view/view.h"

Exam::Exam(Primary *p, QFile &f) : Event(p, "exam", f) {
    parent = p;

    // addition info
    // lesson_ID, building_ID, room
    QString t;
    while (1) {
        t = f.readLine();
        if (t[0] != '#' && t[0] != '\n' && t[0] != '\r') break;
    }

    // lesson_ID
    lesson_ID = t.toInt();

    // building_ID
    t = f.readLine();
    building_ID = t.toInt();

    // room
    t = f.readLine();
    if (t.contains("\r\n"))
        t.chop(2);
    else
        t.chop(1);
    room = t;
}

void Exam::create_and_init_menu(Button *b) {
    // begin_time
    // end_time
    // position(room)
    // opr > navigate
    //     > show lesson details

    // level 1
    b->create_menu(100, 30);

    QString s1 =
        QString("始:%1:%2").arg(begin_time.minute()).arg(begin_time.second());
    auto be_time = new Button(parent, s1, 100, 30);
    be_time->set_mode(Button::label);

    s1 = QString("终:%1:%2").arg(end_time.minute()).arg(end_time.second());
    auto ed_time = new Button(parent, s1, 100, 30);
    ed_time->set_mode(Button::label);

    auto pos = new Button(parent, room, 100, 30);
    pos->set_mode(Button::label);

    auto opr = new Button(parent, "操作>", 100, 30);
    opr->set_mode(Button::men);

    b->menu().add_button(be_time);
    b->menu().add_button(ed_time);
    b->menu().add_button(pos);
    b->menu().add_button(opr);

    // level 2
    opr->create_menu(100, 30);

    auto opr_navigate = new Button(parent, "导航", 100, 30);
    opr_navigate->set_mode(Button::men);

    auto opr_details = new Button(parent, "课程详情", 100, 30);
    opr_details->set_mode(Button::men);

    opr->menu().add_button(opr_navigate);
    opr->menu().add_button(opr_details);

    connect(opr_navigate, &Button::clicked, [=] {
        b->menu().off();
        parent->view->goto_scene(map);
        parent->map->move_to(parent->map->buildings[building_ID], 0, 0);
    });
    
//    connect(opr_details, &Button::clicked, [=] {
//        b->menu().off();
//        parent->view->goto_scene(table);
        // TODO: crash issue here
//        parent->gui->table_gui->selected_lesson_ID = ID;
//    });
}

void Exam::begin_job() {
    QString s = name;
    s.append("开始了.");
    parent->gui->shell->push_msg(s, true);
}

void Exam::end_job() {
    QString s = name;
    s.append("结束了.");
    parent->gui->shell->push_msg(s, true);
}

void Exam::other_job() {
    QString s = name;
    s.append("将要开始,请前往").append(room);
    parent->gui->shell->push_msg(s, true);
}

#include "lesson.h"

#include "code/gui/gui.h"
#include "code/gui/residant/shell/shell.h"
#include "code/gui/table_gui/table_gui.h"
#include "code/map/map.h"
#include "code/primary.h"
#include "code/view/view.h"

Lesson::Lesson(Primary *p, QFile &f) : Event(p, "lesson", f) {
    parent = p;
    // addition info
    // ID, building, room, teacher, group
    QString t;
    while (1) {
        t = f.readLine();
        if (t[0] != '#' && t[0] != '\n' && t[0] != '\r') break;
    }

    // ID
    ID = t.toInt();

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

    // teacher
    t = f.readLine();
    if (t.contains("\r\n"))
        t.chop(2);
    else
        t.chop(1);
    teacher = t;

    // group
    t = f.readLine();
    if (t.contains("\r\n"))
        t.chop(2);
    else
        t.chop(1);
    group = t;
}

int Lesson::lesson_ID() { return ID; }

Building *Lesson::lesson_building() {
    return parent->map->buildings[building_ID];
}

QString Lesson::lesson_room() { return room; }

QString Lesson::lesson_teacher() { return teacher; }

QString Lesson::lesson_group() { return group; }

void Lesson::create_and_init_menu(Button *b) {
    // begin_time
    // end_time
    // teacher
    // position(room)
    // operation > navigate
    //           > details

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

    auto teac = new Button(parent, teacher, 100, 30);
    teac->set_mode(Button::label);

    auto pos = new Button(parent, room, 100, 30);
    pos->set_mode(Button::label);

    auto opr = new Button(parent, "操作>", 100, 30);
    opr->set_mode(Button::men);

    b->menu().add_button(be_time);
    b->menu().add_button(ed_time);
    b->menu().add_button(teac);
    b->menu().add_button(pos);
    b->menu().add_button(opr);

    // level 2
    opr->create_menu(100, 30);

    auto opr_navigate = new Button(parent, "导航", 100, 30);
    opr_navigate->set_mode(Button::men);

    auto opr_details = new Button(parent, "详情", 100, 30);
    opr_details->set_mode(Button::men);

    opr->menu().add_button(opr_navigate);
    opr->menu().add_button(opr_details);

    connect(opr_navigate, &Button::clicked, [=] {
        b->menu().off();
        parent->view->goto_scene(map);
        parent->map->move_to(parent->map->buildings[building_ID], 0, 0);
    });

    connect(opr_details, &Button::clicked, [=] {
        b->menu().off();
        parent->view->goto_scene(table);
        parent->gui->table_gui->selected_lesson_ID = ID;
    });
}

void Lesson::begin_job() {
    QString s("课程");
    s.append(name).append("开始上课.");
    parent->gui->shell->push_msg(s, true);
}

void Lesson::end_job() {
    QString s("课程");
    s.append(name).append("结束了.");
    parent->gui->shell->push_msg(s, true);
}

void Lesson::other_job() {
    QString s("课程");
    s.append(name).append("将要开始,请前往").append(room);
    parent->gui->shell->push_msg(s, true);
}

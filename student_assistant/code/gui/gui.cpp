#include "gui.h"

#include "activity_gui/activity_gui.h"
#include "button.h"
#include "code/map/map.h"
#include "code/primary.h"
#include "code/view/view.h"
#include "map_gui/map_gui.h"
#include "residant/bird.h"
#include "residant/digit_clock.h"
#include "residant/shell/shell.h"
#include "residant/time_controller.h"
#include "table_gui/table_gui.h"

GUI::GUI(Primary* p) : QObject() {
    parent = p;

    // residants
    button_map = new Button(p, QString("地图"), 400, 150);
    button_map->set_ico(":/img/button_ico/map.png");
    button_map->set_mode(Button::swit);
    button_activ = new Button(p, QString("事件"), 400, 150);
    button_activ->set_ico(":/img/button_ico/activ.png");
    button_activ->setPos(0, 150);
    button_activ->set_mode(Button::swit);
    button_table = new Button(p, QString("课表"), 400, 150);
    button_table->set_ico(":/img/button_ico/table.png");
    button_table->setPos(0, 300);
    button_table->set_mode(Button::swit);

    // click one down, others up
    connect(button_map, &Button::clicked,
            [=] { parent->view->goto_scene(map); });
    connect(button_activ, &Button::clicked,
            [=] { parent->view->goto_scene(activ); });
    connect(button_table, &Button::clicked,
            [=] { parent->view->goto_scene(table); });

    // NOTE* these guis are created here, but not added to the scene UNTIL View
    // is created in Primary and calls all_xxx_on(QGraphicsScene &s)
    bird = new Bird(p);
    digit_clock = new DigitClock(p);
    time_controller = new TimeController(p);
    shell = new Shell(p);

    map_gui = new MapGUI(p);
    activ_gui = new ActivityGUI(p);
    table_gui = new TableGUI(p);
}

GUI::~GUI() {
    qDebug("GUI deleted");
    // delete button_map;
    // delete button_activ;
    // delete button_table;
    // delete bird;
    delete digit_clock;
    // delete time_controller;
    delete shell;
    // delete map_gui;
}

// move all residant guis to s
// called in goto_scene_xxx
void GUI::all_residant_on(QGraphicsScene& s) {
    button_map->on(s);
    button_activ->on(s);
    button_table->on(s);

    bird->on(s);
    digit_clock->on(s);
    time_controller->on(s);
    shell->on(s);
}

// called in View()
void GUI::all_map_gui_on(QGraphicsScene& s) {
    map_gui->label_campus->on(s);
    for (int i = 0; i < 2; i++) map_gui->button_campus[i]->on(s);

    // buildings
    for (auto i : parent->map->buildings) i->on(s);

    // paths
    for (auto i : parent->map->paths) s.addItem(i);
    // test
    // map_gui->test_button->on(s);
}

void GUI::all_activ_gui_on(QGraphicsScene& s) {
    activ_gui->label_date->on(s);
    activ_gui->prev_day->on(s);
    activ_gui->next_day->on(s);

    // time_line
    for (int i = 0; i < 4; i++) s.addItem(activ_gui->time_line[i]);

    // label_line
    for (int i = 0; i < 8; i++) s.addItem(activ_gui->label_line[i]);

    // new_alarm
    activ_gui->new_alarm[0]->on(s);
}

void GUI::all_table_gui_on(QGraphicsScene& s) {
    for (int i = 0; i < 11; i++) s.addItem(table_gui->horizontal[i]);
    for (int i = 0; i < 8; i++) s.addItem(table_gui->vertical[i]);

    for (int i = 0; i < 4; i++) s.addItem(table_gui->info[i]);
    for (int i = 0; i < 2; i++) s.addItem(table_gui->search[i]);

    for (int i = 0; i < 7; i++) table_gui->label_week[i]->on(s);
    for (int i = 0; i < 9; i++) table_gui->label_time[i]->on(s);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 7; j++) table_gui->lesson_button[i][j]->on(s);

    for (int i = 0; i < 5; i++) table_gui->lesson_info_button[i]->on(s);
    for (int i = 0; i < 4; i++) table_gui->lesson_work_button[i]->on(s);
    for (int i = 0; i < 3; i++) table_gui->lesson_file_button[i]->on(s);
    for (int i = 0; i < 5; i++) table_gui->lesson_exam_button[i]->on(s);
}

#include "view.h"
#include "code/tool_lib.h"
#include <QDebug>
#include <QIcon>
#include "code/activity/activity.h"
#include "code/gui/activity_gui/activity_gui.h"
#include "code/gui/button.h"
#include "code/gui/gui.h"
#include "code/gui/residant/bird.h"
#include "code/gui/residant/digit_clock.h"
#include "code/gui/residant/shell/shell.h"
#include "code/gui/residant/time_controller.h"
#include "code/primary.h"
#include "code/timetable/timetable.h"

void (*goto_scene_call[])(View*) = {
    [map] = goto_scene_map,
    [activ] = goto_scene_activ,
    [table] = goto_scene_table,
};

View::View(Primary* p) : QGraphicsView() {
    parent = p;
    sce_map = new QGraphicsScene;
    sce_activ = new QGraphicsScene;
    sce_table = new QGraphicsScene;
    auto gui = parent->gui;
    gui->all_map_gui_on(*sce_map);
    gui->all_activ_gui_on(*sce_activ);
    gui->all_table_gui_on(*sce_table);

    setWindowTitle("鹦鹉课表");
    setWindowIcon(QIcon(":/img/ico/ico.png"));
    setFixedSize(APP_WIDTH, APP_HEIGHT);
    setSceneRect(0, 0, APP_WIDTH, APP_HEIGHT);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFont(QFont("times", 15));
    search = new QLineEdit(this);
    search->setGeometry(0, 0, 774, 49);
    search->move(577, 862);
    search->setPlaceholderText("查找课程");
    search->setFrame(false);
    search->hide();
    connect(search, &QLineEdit::textChanged, this, &View::call_do_search);

    show();
    goto_scene(map);
}

View::~View() { qDebug("View deleted"); }

void View::call_do_search(QString s) { parent->time_table->do_search(s); }

QGraphicsScene& View::cur_scene() { return *scene(); }

void View::goto_scene(SCENE ind) {
    if (ind == map && &cur_scene() == sce_map) return;
    if (ind == activ && &cur_scene() == sce_activ) return;
    if (ind == table && &cur_scene() == sce_table) return;

    if (ind == map) {
        setScene(sce_map);
        parent->gui->button_map->down();
        parent->gui->button_activ->up();
        parent->gui->button_table->up();

        search->hide();
    } else if (ind == activ) {
        setScene(sce_activ);
        parent->gui->button_map->up();
        parent->gui->button_activ->down();
        parent->gui->button_table->up();

        search->hide();
    } else if (ind == table) {
        setScene(sce_table);
        parent->gui->button_map->up();
        parent->gui->button_activ->up();
        parent->gui->button_table->down();

        search->show();
    }
    goto_scene_call[ind](this);
}

void View::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Q) parent->gui->time_controller->speed_down();
    if (e->key() == Qt::Key_E) parent->gui->time_controller->speed_up();
    if (e->key() == Qt::Key_1) goto_scene(map);
    if (e->key() == Qt::Key_2) goto_scene(activ);
    if (e->key() == Qt::Key_3) goto_scene(table);
}

bool View::eventFilter(QObject* obj, QEvent* e) {
    // is_able works only in keyPressEvent and mousePressEvent
    if (e->type() == QEvent::Wheel) {
        parent->activ->set_alarm(e);
        return true;
    }
    return QObject::eventFilter(obj, e);
}

void goto_scene_map(View* v) {
    qDebug("scene: map");

    auto gui = v->parent->gui;
    gui->all_residant_on(v->cur_scene());
}
void goto_scene_activ(View* v) {
    qDebug("scene: activ");

    auto gui = v->parent->gui;
    gui->all_residant_on(v->cur_scene());

    // switch day to today when goto this scene
    QTime t = v->parent->gui->digit_clock->current_time();
    v->parent->gui->activ_gui->current_day = t.hour();
}
void goto_scene_table(View* v) {
    qDebug("scene: table");

    auto gui = v->parent->gui;
    gui->all_residant_on(v->cur_scene());
}

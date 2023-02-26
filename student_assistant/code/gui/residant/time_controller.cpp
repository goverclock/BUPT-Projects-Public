#include "time_controller.h"

#include <QDebug>
#include <QFile>
#include <QTime>

#include "bird.h"
#include "code/gui/gui.h"
#include "code/primary.h"
#include "digit_clock.h"
TimeController::TimeController(Primary *p) : QObject(), QGraphicsRectItem() {
    parent = p;

    setRect(0, 0, 200, 200);
    setPos(200, 750);

    for (int i = 0; i < 4; i++) rate_button[i] = new Button(p, "");

    rate_button[0]->set_ico(":/img/time_controller/0x.png");
    rate_button[0]->setParentItem(this);
    rate_button[0]->setPos(0, 0);
    rate_button[0]->set_mode(Button::swit);

    rate_button[1]->set_ico(":/img/time_controller/1x.png");
    rate_button[1]->setParentItem(this);
    rate_button[1]->setPos(100, 0);
    rate_button[1]->set_mode(Button::swit);
    rate_button[1]->down();

    rate_button[2]->set_ico(":/img/time_controller/2x.png");
    rate_button[2]->setParentItem(this);
    rate_button[2]->setPos(0, 100);
    rate_button[2]->set_mode(Button::swit);

    rate_button[3]->set_ico(":/img/time_controller/3x.png");
    rate_button[3]->setParentItem(this);
    rate_button[3]->setPos(100, 100);
    rate_button[3]->set_mode(Button::swit);

    rate = 1;
    connect(rate_button[0], &Button::clicked, this, [=]() {
        all_button_up();
        rate_button[0]->down();
        set_rate(0);
    });
    connect(rate_button[1], &Button::clicked, this, [=]() {
        all_button_up();
        rate_button[1]->down();
        set_rate(1);
    });
    connect(rate_button[2], &Button::clicked, this, [=]() {
        all_button_up();
        rate_button[2]->down();
        set_rate(2);
    });
    connect(rate_button[3], &Button::clicked, this, [=]() {
        all_button_up();
        rate_button[3]->down();
        set_rate(3);
    });
}

void TimeController::speed_up(int n) {
    if (n == 0) return;
    if (rate < 3) set_rate(rate + 1);  // rate is increased here
    all_button_up();
    rate_button[rate]->down();  // not "rate + 1"
    speed_up(n - 1);
}
void TimeController::speed_down(int n) {
    if (n == 0) return;
    if (rate > 0) set_rate(rate - 1);  // rate is decreased here
    all_button_up();
    rate_button[rate]->down();  // not "rate - 1"
    speed_down(n - 1);
}
void TimeController::set_rate(int n) {
    rate = n;
    parent->gui->bird->set_rate(n);
    parent->gui->digit_clock->set_rate(n);
    // parent->map->core_timer->set_rate(n);
    // parent->activ->core_timer->set_rate(n);
    // parent->time_table->core_timer->set_rate(n);
}

void TimeController::all_button_up() {
    for (int i = 0; i < 4; i++) rate_button[i]->up();
}

bool TimeController::is_on() { return _is_on; }

void TimeController::on(QGraphicsScene &s) {
    s.addItem(this);
    _is_on = true;
}

void TimeController::off(QGraphicsScene &s) {
    if (is_on()) s.removeItem(this);
    _is_on = false;
}

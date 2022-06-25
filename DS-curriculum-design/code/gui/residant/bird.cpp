#include "bird.h"

#include <QGraphicsRectItem>
#include <QPixmap>

Bird::Bird(Primary *p) : QObject(), QGraphicsRectItem() {
    parent = p;
    bird_timer = new QTimer(this);

    setRect(0, 0, 200, 200);
    setPos(0, 750);

    QPixmap *pixmap[10];
    for (int i = 0; i < 10; i++) pixmap[i] = new QPixmap;
    pixmap[0]->load(":/img/bird/bird1.png");
    pixmap[1]->load(":/img/bird/bird2.png");
    pixmap[2]->load(":/img/bird/bird3.png");
    pixmap[3]->load(":/img/bird/bird4.png");
    pixmap[4]->load(":/img/bird/bird5.png");
    pixmap[5]->load(":/img/bird/bird6.png");
    pixmap[6]->load(":/img/bird/bird7.png");
    pixmap[7]->load(":/img/bird/bird8.png");
    pixmap[8]->load(":/img/bird/bird9.png");
    pixmap[9]->load(":/img/bird/bird10.png");

    for (int i = 0; i < 10; i++) {
        pix[i] = new QGraphicsPixmapItem(this);
        pix[i]->setPixmap(pixmap[i]->scaled(200, 200));
        pix[i]->setPos(0, 0);
        pix[i]->hide();
    }
    pix[0]->show();

    set_rate(1);
    connect(bird_timer, &QTimer::timeout, this, [&] {
        pix[cur_frame]->hide();
        if (++cur_frame >= 10) cur_frame = 0;
        pix[cur_frame]->show();
    });


}

bool Bird::is_on() { return _is_on; }

void Bird::on(QGraphicsScene &s) {
    s.addItem(this);
    _is_on = true;
}

void Bird::off(QGraphicsScene &s) {
    if (is_on()) s.removeItem(this);
    _is_on = false;
}

void Bird::set_rate(int n) {
    if (n == 0)
        bird_timer->stop();
    else {
        if (bird_timer->isActive() == true) bird_timer->stop();
        bird_timer->start(50 / n);
    }
}

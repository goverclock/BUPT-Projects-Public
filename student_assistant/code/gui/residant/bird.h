#pragma once
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QObject>
#include <QTimer>

class Primary;

class Bird : public QObject, public QGraphicsRectItem {
    Q_OBJECT
   public:
    Bird(Primary *p);

    Primary *parent;

    int cur_frame = 0;
    QGraphicsPixmapItem *pix[10];

    bool is_on();
    void on(QGraphicsScene &);
    void off(QGraphicsScene &);
    void set_rate(int n);

   private:
    QTimer *bird_timer;
    bool _is_on;
};

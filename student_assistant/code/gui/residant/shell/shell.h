#pragma once
#include <QGraphicsRectItem>
#include <QObject>
#include "msg_queue.h"
#include <QFile>
#include <QIODevice>

class Primary;
struct Msg;

class Shell : public QObject, public QGraphicsRectItem {
    Q_OBJECT
   public:
    Shell(Primary *);
    ~Shell();

    Primary *parent;
    MsgQueue *msg_queue;
    QFile shell_log;

    bool is_on();
    void on(QGraphicsScene &);
    void off(QGraphicsScene &);
    void push_msg(QString, bool);
    void clear_msg(bool c = false);
    void set_mode(enum Qt::GlobalColor);
   public slots:
    void push_msg_core(QString, bool);

   private:
    bool _is_on = false;
};

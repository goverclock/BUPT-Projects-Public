#pragma once
#include <QObject>
#include <QString>
#include <QVector>
#include <QTimer>

#include "code/gui/text.h"

class Shell;

#define MAX_MSG 10
enum MSG_t { plain, percent_bar };
struct Msg {
    MSG_t type;
    QString plain_text;
    int val;  // if type == percent_bar, it represents percentage, then negative
              // is allowed
    int lag;
};

class MsgQueue : public QObject {
    Q_OBJECT
   public:
    MsgQueue(Shell *);

    Shell *parent;

    void push_msg(const Msg);
    void clear_msg(bool c = false);
    void set_mode(enum Qt::GlobalColor);
   public slots:
    void update();
   signals:
    void percent_to_100(const QString);

   private:
    QVector<Msg> q;
    QVector<Msg> buf;
    Text *msg[MAX_MSG];
    QTimer *msg_queue_timer;
};

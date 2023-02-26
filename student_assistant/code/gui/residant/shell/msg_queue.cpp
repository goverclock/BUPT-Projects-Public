#include "msg_queue.h"

#include <QDebug>
#include <QFont>
#include <QTimer>
#include "code/primary.h"
#include "shell.h"
#include "code/gui/gui.h"

MsgQueue::MsgQueue(Shell* p) : QObject() {
    parent = p;

    // initialize msg queue
    for (int i = 0; i < MAX_MSG; i++) {
        msg[i] = new Text(parent, "");
        auto m = msg[i];
        m->setParent(parent);
        m->setDefaultTextColor(Qt::white);
        m->set_weight(QFont::Thin);
        m->set_point_size(10);
        m->setPos(5, 30 * i);
    }

    // refresh per frame
    msg_queue_timer = new QTimer(this);
    msg_queue_timer->start(10);
    connect(msg_queue_timer, &QTimer::timeout, this, &MsgQueue::update);
}

// push to buffer, when lag is over, push to shell
// note:only the front's lag is reduced in update(), so lag is accumulated
void MsgQueue::push_msg(const Msg m) { buf.push_back(m); }

void MsgQueue::clear_msg(bool clear_buffer) {
    q.clear();
    if (clear_buffer) buf.clear();
    for (int i = 0; i < MAX_MSG; i++) msg[i]->set_text("");
}

void MsgQueue::set_mode(enum Qt::GlobalColor c) {
    for(int i = 0; i < MAX_MSG; i++)
        msg[i]->setDefaultTextColor(c);
}

void MsgQueue::update() {
    // update lag
    while (!buf.empty()) {
        if (--buf.front().lag > 0) break;
        auto m = buf.front();
        buf.pop_front();
        q.push_back(m);
        if (q.size() > MAX_MSG) q.pop_front();
    }

    // refresh shell
    int ind = 0;
    for (auto& m : q) {
        if (m.type == plain)
            msg[ind]->set_text(m.plain_text);
        else if (m.type == percent_bar) {
            int cnt = (m.val / 5) > 0 ? (m.val / 5) : 0;
            int num = m.val;
            QString t(m.plain_text);
            t.append(" [");
            for (int i = 1; i <= cnt; i++) t.append("■");
            for (int i = 1; i <= 20 - cnt; i++) t.append("□");
            t.append("] ");
            num = std::max(num, 0);
            num = std::min(num, 100);
            t.append(QString::number(num));
            t.append("%");

            if (m.val <= 100) m.val++;
            if (m.val == 100) emit percent_to_100(m.plain_text);
            msg[ind]->set_text(t);
        }
        ind++;
    }
}

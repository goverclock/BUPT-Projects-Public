#include "shell.h"
#include <QDir>

#include <QDebug>
#include <QString>
#include <QTime>

#include "code/gui/gui.h"
#include "code/gui/residant/digit_clock.h"
#include "code/map/map.h"
#include "code/primary.h"
#include "code/tool_lib.h"
#include "code/view/view.h"

Shell::Shell(Primary* p) : QObject(), QGraphicsRectItem() {
    parent = p;
    connect(parent->map, &Map::core_msg, this, &Shell::push_msg_core);

    setRect(0, 0, 400, 300);
    setPos(0, 450);
    setPen(QPen(Qt::black, 0.5));
    setBrush(Qt::black);

    shell_log.setFileName(cwd() + "/saves/shell.log");
    if (!shell_log.open(QIODevice::ReadWrite | QIODevice::Text |
                        QIODevice::Append))
        panic("Fail to open shell.log");

    msg_queue = new MsgQueue(this);
    set_mode(Qt::white);
}

Shell::~Shell() {
    qDebug("Shell deleted");
    shell_log.close();
}

bool Shell::is_on() { return _is_on; }

void Shell::on(QGraphicsScene& s) {
    s.addItem(this);
    _is_on = true;
}

void Shell::off(QGraphicsScene& s) {
    if (is_on()) s.removeItem(this);
    _is_on = false;
}

void Shell::push_msg(QString m, bool has_time) {
    // add time as header
    QTime ti = parent->gui->digit_clock->current_time();
    QString tis = QString("[%1:%2]")
                      .arg(QString::number(ti.minute()))
                      .arg(QString::number(ti.second()));
    if (has_time) m.prepend(tis);

    // save to log
    QString t = m + "\n";
    shell_log.write(t.toUtf8());

    if (m.size() < 22) {
        msg_queue->push_msg(Msg{plain, m, 0, 0});
        return;
    }
    QString front_m;
    int chop_len = m.size() - 21;
    front_m = m.chopped(chop_len);
    msg_queue->push_msg(Msg{plain, front_m, 0, 0});
    m.remove(0, 21);
    msg_queue->push_msg(Msg{plain, m, 0, 0});
}

void Shell::clear_msg(bool clear_buffer) { msg_queue->clear_msg(clear_buffer); }

void Shell::set_mode(enum Qt::GlobalColor c) {
    if (c == Qt::black)
        msg_queue->set_mode(Qt::white);
    else if (c == Qt::white)
        msg_queue->set_mode(Qt::black);

    setBrush(c);
}

void Shell::push_msg_core(QString m, bool t) { push_msg(m, t); }

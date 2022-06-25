#include "code/tool_lib.h"

#include <QDebug>
#include <QDir>
#include <QString>

QPointF center_pos(int aw, int ah, int bw, int bh) {
    QPointF ret;
    ret.setX(aw / 2 - bw / 2);
    ret.setY(ah / 2 - bh / 2);
    return ret;
}

void panic(QString s) {
    qDebug() << "panic:" << s;
    for (;;)
        ;
}

QTime add_minute(QTime x, int n) {
    QTime t = x.addSecs(n);
    int d = t.hour(), h = t.minute(), m = t.second();
    while (h >= 24) {
        h -= 24;
        d++;
    }
    while (d > 7) d -= 7;
    t.setHMS(d, h, m);
    return t;
}

QString cwd() {
    // return QString("./document");
    return QString("..");
}

QString cwd_win() { 
    // return QString(".\\document"); 
    return QString("..");
}

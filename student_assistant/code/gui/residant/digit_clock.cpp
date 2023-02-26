#include "digit_clock.h"
#include <QDir>
#include <QFile>
#include <QString>

#include "code/activity/activity.h"
#include "code/timetable/timetable.h"
#include "code/gui/text.h"
#include "code/primary.h"
#include "code/tool_lib.h"
#include "code/view/view.h"

DigitClock::DigitClock(Primary *p) : QObject(), QGraphicsRectItem() {
    parent = p;

    setRect(0, 0, 80, 35);
    setPen(QPen(Qt::transparent));  // make the rect invisible
    setPos(60, 750);
    digit_clock_timer = new QTimer(this);

    QTime *cur_time = new QTime(QTime::currentTime());

    QString str = QString("%1:%2").arg(hour).arg(min);
    str.prepend("时间 ");
    text = new Text(this, str);
    text->set_point_size(16);
    text->setPos(-50, 3);


    QFile file(cwd() + "/saves/time.dat");

    int a[3];
    QString file_str;
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write("1\n0\n0\n");
        file.close();
    }
    file.open(QIODevice::ReadOnly);
    for (int cnt = 0; cnt < 3; cnt++) {
        file_str = file.readLine();
        file_str.chop(1);
        a[cnt] = file_str.toInt();
    }

    time.setHMS(a[0], a[1], a[2]);
    set_time(time);

    file.close();

    set_rate(1);
    connect(digit_clock_timer, &QTimer::timeout, this, [&]() {
        min++;
        if (min == 60) {
            min = 0;
            if (hour == 23) {
                hour = 0;
                if (week == 7) {
                    week = 1;
                } else
                    week++;
            } else
                hour++;
        }

        QString str =
            QString("%1 %2:%3").arg(weeks[week - 1]).arg(hour).arg(min);
        str.prepend("周");
        text->setPlainText(str);
        text->show();

        emit time_tick(current_time());
    });

    connect(this, &DigitClock::time_tick, parent->activ, &Activity::update);
    connect(this, &DigitClock::time_tick, parent->time_table, &TimeTable::notify);
}

DigitClock::~DigitClock() {
    qDebug("DigitClock deleted");
    QFile file(cwd() + "/saves/time.dat");
    QString str = QString("%1\n%2\n%3\n").arg(week).arg(hour).arg(min);
    char *ch;
    QByteArray arry;

    arry = str.toLatin1();
    ch = arry.data();
    file.open(QFile::WriteOnly);
    file.write(ch);
    file.close();
}

bool DigitClock::is_on() { return _is_on; }

void DigitClock::on(QGraphicsScene &s) {
    s.addItem(this);
    _is_on = true;
}

void DigitClock::off(QGraphicsScene &s) {
    if (is_on()) s.removeItem(this);
    _is_on = false;
}

QTime DigitClock::current_time() { return QTime(week, hour, min); }

void DigitClock::set_rate(int n) {
    if (n == 0)
        digit_clock_timer->stop();
    else {
        if (digit_clock_timer->isActive() == true) digit_clock_timer->stop();
        digit_clock_timer->start(50 / n);
    }
}
void DigitClock::set_time(QTime time) {
    week = time.hour();
    hour = time.minute();
    min = time.second();
}

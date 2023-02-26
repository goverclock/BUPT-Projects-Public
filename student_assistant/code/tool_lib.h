#pragma once
#include <QDir>
#include <QString>
#include <QObject>
#include <QPointF>
#include <QTime>

// to put B in the center of A:
// B.setPos(centerpos(aw, ah, bw, bh))
QPointF center_pos(int aw, int ah, int bw, int bh);
void panic(QString);
QTime add_minute(QTime, int);
QString cwd();
QString cwd_win();

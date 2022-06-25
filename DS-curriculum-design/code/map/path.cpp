#include "path.h"

#include <QDebug>
#include <QLine>
#include <QString>
#include <algorithm>
#include <cmath>
#include <QDebug>

#include "building/building.h"
#include "code/primary.h"
#include "code/tool_lib.h"
#include "map.h"

Path::Path(Primary *p, QString t, QVector<Building *> &bs,
           bool reverse_building) {
    parent = p;

    // ls: Building_A, Building_B, crowding ,can_bike
    QStringList ls = t.split(QString(" "));
    from = ls[0].toInt();
    to = ls[1].toInt();
    if (reverse_building) std::swap(from, to);
    crowding = ls[2].toDouble();
    _can_bike = ls[3].toInt();
    ideal = 10;
    _campus = bs[from]->campus();

    auto pos_from = bs[from]->position();
    auto pos_to = bs[to]->position();
    QLine ln(pos_from, pos_to);
    dist = std::sqrt(ln.dx() * ln.dx() + ln.dy() * ln.dy());

    auto x_off = bs[from]->x_off;
    setLine(pos_from.x() - x_off, pos_from.y(), pos_to.x() - x_off, pos_to.y());
    setZValue(1);

    QPen pen;
    pen.setWidth(3);
    if (_can_bike)
        pen.setColor(Qt::black);
    else
        pen.setColor(Qt::gray);
    setPen(pen);

    // qDebug("%d", dist);

    // building
    bs[from]->add_out_path(this);
}

void Path::set_highlight(bool h) {
    QPen pen;
    pen.setWidth(3);

    if (h)
        pen.setColor(Qt::red);
    else if (_can_bike)
        pen.setColor(Qt::black);
    else
        pen.setColor(Qt::gray);

    setPen(pen);
}

int Path::distance() { return dist; }

int Path::time_cost() {
    int t = crowding * ideal;
    if (!t) panic("Path: time_cost == 0");
    return 1000 / t;
}

int Path::campus() { return _campus; }

Building *Path::from_building() { return parent->map->buildings[from]; }
Building *Path::to_building() { return parent->map->buildings[to]; }

void Path::set_reverse_path(Path *rev_p) { _reverse_path = rev_p; }
Path *Path::rever_path() { return _reverse_path; }
bool Path::can_bike() { return _can_bike; }

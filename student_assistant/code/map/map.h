#pragma once
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QVector>

#include "building/building.h"
#include "path.h"

class Primary;

// CORE for MapGUI in code/gui/map
// DO NOT CALL function implemented in code/gui
// everything is public here, so that easy to access
class Map : public QObject {
    Q_OBJECT
   public:
    Map(Primary *);
    ~Map();

    Primary *parent;

    QVector<Building *> buildings;
    QVector<Path *> paths;
    int current_building;
    int current_campus = 0;
    QGraphicsPixmapItem *pix;



    void set_campus(int n);
    void move_to(Building *, int, bool);
   signals:
    void core_msg(QString, bool);

};

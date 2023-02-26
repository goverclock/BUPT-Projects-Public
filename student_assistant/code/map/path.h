#pragma once
#include <QGraphicsLineItem>
#include <QObject>

class Primary;
class Building;

class Path : public QObject, public QGraphicsLineItem{
    Q_OBJECT
   public:
    Path(Primary *p, QString t, QVector<Building*> &bs, bool reverse_building);

    Primary *parent;

    void set_highlight(bool);
    int distance();
    int time_cost();
    int campus();
    Building *from_building();
    Building *to_building();
    void set_reverse_path(Path*);
    Path *rever_path();
    bool can_bike();

   private:
    int from, to;             // ID of Building
    int dist;
    double crowding, ideal;   // real = crowding*ideal
    bool _can_bike;
    int _campus;
    Path *_reverse_path;
};

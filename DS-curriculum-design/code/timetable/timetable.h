#pragma once
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QObject>
#include <QVector>

class Primary;

class TimeTable: public QObject {
    Q_OBJECT
   public:
    TimeTable(Primary*);
    ~TimeTable();

    void notify(QTime);

    Primary *parent;

    QVector<int> search_result;

   public slots:
    void do_search(QString);
};

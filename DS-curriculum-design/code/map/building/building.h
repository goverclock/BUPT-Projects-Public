#pragma once
#include <QFile>
#include <QVector>
#include "code/gui/button.h"

class Primary;
class Path;

class Building : public Button {
   public:
    Building(Primary *, QFile &);

    Primary *parent;

    const int x_off = 400;
    QString name();
    void set_name(QString);
    int ID();
    void set_ID(int);
    QPoint position();
    void set_position(int, int);
    int campus();
    void set_campus(int);
    QVector<Path *> &out_paths();
    void add_out_path(Path*);

   private:
    QVector<Path*> _out_paths;
    QString _name;
    int _campus;
    int _ID;
};

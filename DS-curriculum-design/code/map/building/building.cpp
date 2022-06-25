#include "building.h"

#include <QDebug>

#include "code/gui/gui.h"
#include "code/map/map.h"
#include "code/primary.h"

Building::Building(Primary* p, QFile& f) : Button(p) {
    parent = p;
    setZValue(2);

    // read from file to initialize
    int ind = 0;
    QString t;
    while (ind < 4) {
        t = f.readLine();
        if (t[0] == '#' || t[0] == '\n') continue;
        if (t.contains("\r\n"))
            t.chop(2);
        else
            t.chop(1);

        if (ind == 0)  // ID
            set_ID(t.toInt());
        else if (ind == 1) {  // name
            set_name(t);
            // **********DEBUG********* REMOVE AFTER USE
            // t.append(QString::number(ID()));
            // set_name(t);
            // ************************
        } else if (ind == 2) {  // x, y, w, h
            int x, y, w, h;
            x = t.toInt();
            x += 50;    // stupid offsets

            t = f.readLine();
            if (t.contains("\r\n"))
                t.chop(2);
            else
                t.chop(1);
            y = t.toInt();

            t = f.readLine();
            if (t.contains("\r\n"))
                t.chop(2);
            else
                t.chop(1);
            w = t.toInt();
            t = f.readLine();
            if (t.contains("\r\n"))
                t.chop(2);
            else
                t.chop(1);
            h = t.toInt();
            set_size(w, h);  // set size first, then position
            set_position(x, y);
        } else if (ind == 3)  // campus
            set_campus(t.toInt());

        ind++;
    }
    f.readLine();  // eat empty line

    set_mode(Button::label);
    set_point_size(10);

    // menu
    if(name() == " ") return;   // no menu for waypoint
    // level 1
    create_menu(110, 30);
    Button* walk_b = new Button(parent, "步行>");
    Button* bike_b = new Button(parent, "自行车>");
    walk_b->set_mode(Button::men);
    bike_b->set_mode(Button::men);
    menu().add_button(walk_b);
    menu().add_button(bike_b);

    // level 2
    walk_b->create_menu(110, -1);
    bike_b->create_menu(110, -1);
    Button* walk_time_b = new Button(parent, "最短时间");
    Button* walk_dist_b = new Button(parent, "最短距离");
    Button* bike_time_b = new Button(parent, "最短时间");
    Button* bike_dist_b = new Button(parent, "最短距离");
    walk_time_b->set_mode(Button::men);
    walk_dist_b->set_mode(Button::men);
    bike_time_b->set_mode(Button::men);
    bike_dist_b->set_mode(Button::men);
    walk_b->menu().add_button(walk_time_b);
    walk_b->menu().add_button(walk_dist_b);
    bike_b->menu().add_button(bike_time_b);
    bike_b->menu().add_button(bike_dist_b);

    connect(walk_time_b, &Button::clicked,
            [=] { parent->map->move_to(this, 0, false); });
    connect(walk_dist_b, &Button::clicked,
            [=] { parent->map->move_to(this, 1, false); });
    connect(bike_time_b, &Button::clicked,
            [=] { parent->map->move_to(this, 0, true); });
    connect(bike_dist_b, &Button::clicked,
            [=] { parent->map->move_to(this, 1, true); });
}

QString Building::name() { return _name; }

void Building::set_name(QString na) {
    _name = na;
    set_text(na);
}

int Building::ID() { return _ID; }

void Building::set_ID(int n) { _ID = n; }

QPoint Building::position() {
    return QPoint(x() + rect().width() / 2 + x_off, y() + rect().height() / 2);
}

void Building::set_position(int x, int y) {
    setPos(x - rect().width() / 2 + x_off, y - rect().height() / 2);
}

int Building::campus() { return _campus; }

// *different from Map::set_campus()
void Building::set_campus(int n) { _campus = n; }

QVector<Path*>& Building::out_paths() { return _out_paths; }
void Building::add_out_path(Path* p) { _out_paths.push_back(p); }

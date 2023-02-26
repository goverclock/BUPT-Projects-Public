#include "map_gui.h"

#include "code/gui/gui.h"
#include "code/gui/residant/shell/shell.h"
#include "code/gui/text.h"
#include "code/map/map.h"
#include "code/primary.h"

MapGUI::MapGUI(Primary *p) : QObject() {
    parent = p;
    map_gui_timer = new QTimer(this);
    map_gui_timer->start(20);
    connect(map_gui_timer, &QTimer::timeout, this, &MapGUI::update);

    // note: after "new", checkout GUI::all_map_gui_on(QGraphicsScene& s)
    label_campus = new Button(p, "校区", 100, 50);
    label_campus->set_mode(Button::label);
    label_campus->setPos(400 + 375, 0);
    label_campus->setZValue(1);
    label_campus->set_point_size(15);
    label_campus->set_edge(false);

    button_campus[0] = new Button(p, "A", 100, 50);
    button_campus[0]->set_mode(Button::swit);
    button_campus[0]->setPos(400 + 475, 0);
    button_campus[0]->setZValue(1);
    button_campus[0]->set_point_size(20);

    button_campus[1] = new Button(p, "B", 100, 50);
    button_campus[1]->set_mode(Button::swit);
    button_campus[1]->setPos(400 + 575, 0);
    button_campus[1]->setZValue(1);
    button_campus[1]->set_point_size(20);
    for (int i = 0; i < 2; i++)
        connect(button_campus[i], &Button::clicked,
                [=] { parent->map->set_campus(i); });
}

void MapGUI::update() {
    // campus button
    all_button_campus_up();
    button_campus[parent->map->current_campus]->down();

    // buildings
    int c_cam = parent->map->current_campus;
    auto &bds = parent->map->buildings;
    if (bds.first()->campus() != c_cam || bds.first()->isVisible() == false) {
        // campus is changed, change building's visibility
        for (auto i : bds)
            if (i->campus() == c_cam)
                i->show();
            else
                i->hide();
    }

    // paths
    auto &pts = parent->map->paths;
    if (pts.first()->campus() != c_cam || pts.first()->isVisible() == false)
        for (auto i : pts)
            if (i->campus() == c_cam)
                i->show();
            else
                i->hide();

    // my location icon
    auto my = parent->map->pix;
    my->setParentItem(parent->map->buildings[parent->map->current_building]);
    my->setVisible(parent->map->buildings[parent->map->current_building]->isVisible());
    my->setPos(0, 0);
}

void MapGUI::all_button_campus_up() {
    for (int i = 0; i < 2; i++) button_campus[i]->up();
}

#include "primary.h"

#include "code/gui/gui.h"
#include "code/map/map.h"
#include "code/activity/activity.h"
#include "code/timetable/timetable.h"
#include "code/view/view.h"
#include <QDir>
#include <QDebug>

Primary::Primary() {
    // core
    map = new Map(this);
    activ = new Activity(this);
    time_table = new TimeTable(this);

    // gui
    gui = new GUI(this);
    view = new View(this);
}

Primary::~Primary() {
    qDebug("Primary deleted");
    delete map;
    delete activ;
    // delete time_table;

    delete gui;
    delete view;
}

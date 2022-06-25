#pragma once
#include <QTimer>

class View;
class Activity;
class GUI;
class Map;
class TimeTable;

class Primary {
   public:
    Primary();
    ~Primary();

    // core
    Activity *activ;
    Map *map;
    TimeTable *time_table;

    // GUI
    GUI *gui;
    View *view;
};

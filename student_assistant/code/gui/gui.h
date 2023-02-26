#pragma once
#include <QGraphicsItem>
#include <QObject>
#include <QTimer>

class Primary;
class Button;
class Bird;
class DigitClock;
class TimeController;
class Shell;
class MapGUI;
class ActivityGUI;
class TableGUI;

class GUI : public QObject{
    Q_OBJECT
   public:
    GUI(Primary *);
    ~GUI();

    Primary *parent;
    
    // residant GUIs, appear on all scenes
    // ZValue = 0
    Button *button_map;
    Button *button_activ;
    Button *button_table;
    Bird *bird;
    DigitClock *digit_clock;
    TimeController *time_controller;
    Shell *shell;

    // ZValue = 1
    // sce_map's GUIs
    MapGUI *map_gui;
    ActivityGUI *activ_gui;
    TableGUI *table_gui;

    void all_residant_on(QGraphicsScene&);
    void all_map_gui_on(QGraphicsScene&);
    void all_activ_gui_on(QGraphicsScene&);
    void all_table_gui_on(QGraphicsScene&);
};

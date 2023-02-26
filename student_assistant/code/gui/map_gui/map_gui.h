#pragma once
#include <QGraphicsItem>
#include <QObject>
#include "code/gui/button.h"

class Primary;

// depends on code/map/map.cpp as core
class MapGUI : public QObject {
    Q_OBJECT
   public:
    MapGUI(Primary *);

    Primary* parent;

    // ZValue = 1
    Button *label_campus;
    Button *button_campus[2];
    
   public slots:
    void update();
   private:
    QTimer *map_gui_timer;
    void all_button_campus_up();
};

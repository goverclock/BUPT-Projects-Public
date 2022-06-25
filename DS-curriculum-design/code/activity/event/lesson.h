#pragma once
#include <QFile>
#include "code/gui/button.h"
#include "code/map/building/building.h"
#include "event.h"

class Primary;

class Lesson: public Event {
   public:
    Lesson(Primary*, QFile&);

    Primary *parent;

    int ID;
    int building_ID;
    QString room;   // of building
    QString teacher;
    QString group;

    int lesson_ID() override;
    Building* lesson_building() override;
    QString lesson_room() override;
    QString lesson_teacher() override;
    QString lesson_group() override;
    
    void create_and_init_menu(Button *) override;
    void begin_job() override;
    void end_job() override;
    void other_job() override;
};

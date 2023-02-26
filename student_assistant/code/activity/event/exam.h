#pragma once
#include <QFile>

#include "code/gui/button.h"
#include "event.h"

class Primary;

class Exam : public Event {
   public:
    Exam(Primary *, QFile &);

    Primary *parent;

    int lesson_ID;
    int building_ID;
    QString room;

    void create_and_init_menu(Button *) override;
    void begin_job() override;
    void end_job() override;
    void other_job() override;
};

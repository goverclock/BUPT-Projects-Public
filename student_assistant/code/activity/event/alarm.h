#pragma once
#include <QFile>
#include "code/gui/button.h"
#include "event.h"

class Primary;

class Alarm : public Event {
   public:
    Alarm(Primary *, QFile &);
    Alarm(Primary*, EventInfo);

    Primary *parent;

    void create_and_init_menu(Button *) override;
    void begin_job() override;
    // void end_job() override;
    // void other_job() override;
};

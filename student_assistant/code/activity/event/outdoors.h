#pragma once
#include <QFile>
#include "code/gui/button.h"
#include "event.h"

class Primary;

class Outdoors : public Event {
   public:
    Outdoors(Primary *, QFile &);

    Primary *parent;
    QString sub_type;
    int building_ID;
    bool group; // personal(false) or group(true)

    void create_and_init_menu(Button *) override;
    void begin_job() override;
    void end_job() override;
    // void other_job() override;
};

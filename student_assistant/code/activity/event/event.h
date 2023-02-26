#pragma once
#include <QFile>
#include <QString>
#include <QTime>

#include "code/gui/button.h"
#include "code/map/building/building.h"
#include "code/tool_lib.h"

class Primary;

struct EventInfo {
    QString type;
    QString name;
    QTime begin_time;
    QTime end_time;
    QTime other_time;
    int period;
};

// meant to be abstract, DO NOT USE DIRECTLY
class Event : public Button {
   public:
    Event(Primary *p, QString typ,
          QFile &f);  // read from file, for lesson and outdoors
    Event(Primary *, EventInfo);
    ~Event();

    Primary *parent;

    enum PERIOD { none, week, day, hour };
    enum EVENT_STATUS { use, pending, invalid };
    int ID = -1;  // initialized in Activity::add_event()
    QString type;
    QString name;
    QTime begin_time;
    QTime end_time;
    QTime other_time;
    PERIOD period;
    EVENT_STATUS status;

    int duration_minutes();
    bool match_begin(QTime);
    bool match_end(QTime);
    bool match_other(QTime);

    virtual int lesson_ID() {
        panic("Event::lesson_ID(): this event is not a lesson.");
        return -1;
    };
    virtual Building *lesson_building() {
        panic("Event::lesson_building(): this event is not a lesson.");
        return nullptr;
    };
    virtual QString lesson_room() {
        panic("Event::lesson_room(): this event is not a lesson.");
        return "";
    }
    virtual QString lesson_teacher() {
        panic("Event::lesson_teacher(): this event is not a lesson.");
        return "";
    }
    virtual QString lesson_group() {
        panic("Event::lesson_group(): this event is not a lesson.");
        return "";
    }
    virtual void create_and_init_menu(Button *){};
    virtual void begin_job(){};
    virtual void end_job(){};
    virtual void other_job(){};

   private:
    bool match_time(QTime, QTime, PERIOD);
};

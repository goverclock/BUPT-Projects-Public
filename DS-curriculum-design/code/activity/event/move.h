#pragma once
#include <QTime>
#include <QVector>

#include "code/map/path.h"
#include "event.h"

class Primary;

class Move : public Event {
   public:
    Move(Primary *, EventInfo, QVector<Path *>, int);

    Primary *parent;

    Path *current_path;
    QVector<Path *> solution_paths;
    int strategy;

    void begin_job() override;
    void end_job() override;
    // void other_job() override;
};

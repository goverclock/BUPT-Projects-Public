#pragma once
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>

#include "code/activity/event/event.h"
#include "code/gui/button.h"

class Primary;

class TableGUI : public QObject {
    Q_OBJECT
   public:
    TableGUI(Primary *);

    Primary *parent;

    QGraphicsLineItem *horizontal[11];
    QGraphicsLineItem *vertical[8];
    QGraphicsRectItem *info[4];
    Button *search[2];
    Button *label_week[7];
    Button *label_time[9];
    Button *lesson_button[9][7];
    Event *lesson_eves[9][7] = {0};
    Event *lesson_ID_to_eve[100];
    int selected_lesson_ID = -1;

    Button *lesson_info_button[5];
    Button *lesson_work_button[4];
    Button *lesson_file_button[3];
    Button *lesson_exam_button[5];

   public slots:
    void update();

   private:
    QTimer *table_gui_timer;
    void all_button_lesson_up();
    bool check_undone_assignments(QString);
};

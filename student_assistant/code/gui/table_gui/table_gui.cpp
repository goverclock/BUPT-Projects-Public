#include "table_gui.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QPen>
#include <QProcess>
#include <cstdlib>

#include "code/timetable/timetable.h"
#include "code/activity/activity.h"
#include "code/activity/event/exam.h"
#include "code/activity/event/lesson.h"
#include "code/gui/gui.h"
#include "code/gui/residant/shell/shell.h"
#include "code/primary.h"
#include "code/view/view.h"

TableGUI::TableGUI(Primary *p) {
    parent = p;
    table_gui_timer = new QTimer(this);
    table_gui_timer->start(20);
    connect(table_gui_timer, &QTimer::timeout, this, &TableGUI::update);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::black);

    // chart
    for (int i = 0; i < 11; i++) {
        horizontal[i] = new QGraphicsLineItem;
        auto &h = horizontal[i];
        h->setPen(pen);
        h->setZValue(1);
        h->setLine(0, 0, 875, 0);
        h->setPos(400 + 175, 50 + 55 * i);
    }
    for (int i = 0; i < 8; i++) {
        vertical[i] = new QGraphicsLineItem;
        auto &v = vertical[i];
        v->setPen(pen);
        v->setZValue(1);
        v->setLine(0, 0, 0, 550);
        v->setPos(400 + 175 + 125 * i, 50);
    }

    // info
    for (int i = 0; i < 4; i++) {
        info[i] = new QGraphicsRectItem;
        info[i]->setZValue(1);
    }
    info[0]->setRect(0, 0, 245, 200);
    info[0]->setPos(400 + 175, 650);
    info[1]->setRect(0, 0, 210, 200);
    info[1]->setPos(400 + 175 + 245, 650);
    info[2]->setRect(0, 0, 210, 200);
    info[2]->setPos(400 + 175 + 245 + 210, 650);
    info[3]->setRect(0, 0, 210, 200);
    info[3]->setPos(400 + 175 + 245 + 210 + 210, 650);

    // search
    for (int i = 0; i < 2; i++) {
        search[i] = new Button(parent, "", 875, 50);
        search[i]->setZValue(1);
    }
    search[0]->setPos(400 + 175, 860);
    search[0]->set_mode(Button::label);
    search[1]->set_mode(Button::label);
    search[1]->set_size(100, 50);
    search[1]->setPos(400 + 175 + 775, 860);
    search[1]->set_ico(":/img/button_ico/search.png");
    search[1]->set_edge(false);
    search[1]->setBrush(Qt::transparent);
    search[1]->set_point_size(15);

    // lable_week
    QString s("一二三四五六日");
    for (int i = 0; i < 7; i++) {
        QString t("周");
        t.append(s[i]);
        label_week[i] = new Button(parent, t, 125, 55);
        label_week[i]->setZValue(1);
        label_week[i]->set_point_size(15);
        label_week[i]->setPos(400 + 175 + 125 * i, 50);
        label_week[i]->set_mode(Button::label);
    }

    // label time
    QString sx[9] = {"8:00-8:45",   "8:50-9:35",   "9:50-10:35",
                     "10:40-11:25", "13:00-13:45", "13:50-14:35",
                     "14:45-15:30", "15:30-16:25", "16:35-17:20"};
    for (int i = 0; i < 9; i++) {
        auto &lt = label_time[i];
        lt = new Button(parent, sx[i], 125, 55);
        lt->set_point_size(12);
        lt->setZValue(1);
        lt->setPos(400 + 25, 105 + i * 55);
        lt->set_mode(Button::label);
        lt->set_edge(false);
    }

    // lesson buttons
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 7; j++) {
            auto &les = lesson_button[i][j];
            les = new Button(parent, " ", 125, 55);
            les->setZValue(1);
            les->setPos(400 + 175 + j * 125, 105 + i * 55);
            connect(les, &Button::clicked, this, [=] {
                if (lesson_eves[i][j] == nullptr)
                    selected_lesson_ID = -1;
                else
                    selected_lesson_ID = lesson_eves[i][j]->lesson_ID();
            });
        }

    // lesson info button
    for (int i = 0; i < 5; i++) {
        auto &lib = lesson_info_button[i];
        lib = new Button(parent, "", 245, 40);
        lib->set_mode(Button::label);
        lib->setZValue(1);
        lib->setPos(400 + 175, 650 + 40 * i);
        lib->set_edge(false);
        lib->setBrush(Qt::transparent);
    }
    lesson_info_button[0]->set_text("课程");
    lesson_info_button[0]->set_point_size(10);

    // lesson work button
    for (int i = 0; i < 4; i++) {
        auto &lwb = lesson_work_button[i];
        lwb = new Button(parent, "", 150, 40);
        lwb->setZValue(1);
        lwb->set_point_size(10);
    }
    lesson_work_button[0]->set_mode(Button::label);
    lesson_work_button[0]->set_size(210, 40);
    lesson_work_button[0]->set_text("作业");
    lesson_work_button[0]->set_point_size(10);
    lesson_work_button[0]->setPos(400 + 175 + 245, 650);
    lesson_work_button[0]->set_edge(false);
    lesson_work_button[0]->setBrush(Qt::transparent);

    lesson_work_button[1]->set_text("已交作业");
    lesson_work_button[1]->set_point_size(10);
    lesson_work_button[1]->setPos(400 + 175 + 245 + 30, 650 + 50);
    connect(lesson_work_button[1], &Button::clicked, [=] {
        if (selected_lesson_ID == -1) {
            parent->gui->shell->push_msg("[课表]请先选择一门课程.", false);
            return;
        }

        QProcess proc;
        QString path = cwd_win() + "\\saves\\handin\\";
        path.append(lesson_ID_to_eve[selected_lesson_ID]->name);

        proc.start("explorer", QStringList(path));
        proc.waitForFinished();
    });

    lesson_work_button[2]->set_text("未交作业");
    lesson_work_button[2]->set_point_size(10);
    lesson_work_button[2]->setPos(400 + 175 + 245 + 30, 650 + 50 * 2);
    connect(lesson_work_button[2], &Button::clicked, [=] {
        if (selected_lesson_ID == -1) {
            parent->gui->shell->push_msg("[课表]请先选择一门课程.", false);
            return;
        }

        QString lesson_name = lesson_ID_to_eve[selected_lesson_ID]->name;
        if (check_undone_assignments(lesson_name)) return;

        QProcess proc;
        QString path = cwd_win() + "\\saves\\assignments\\";
        path.append(lesson_name);

        proc.start("explorer", QStringList(path));
        proc.waitForFinished();
    });

    lesson_work_button[3]->set_text("提交作业");
    lesson_work_button[3]->set_mode(Button::label);
    lesson_work_button[3]->set_point_size(10);
    lesson_work_button[3]->setPos(400 + 175 + 245 + 30, 650 + 50 * 3);
    connect(lesson_work_button[3], &Button::clicked, [=] {
        if (selected_lesson_ID == -1) {
            parent->gui->shell->push_msg("[课表]请先选择一门课程.", false);
            return;
        }

        QString lesson_name = lesson_ID_to_eve[selected_lesson_ID]->name;

        // select a file through a pop up window
        QString fname = QFileDialog::getOpenFileName(
            parent->view, QStringLiteral("提交作业"));
        QFileInfo finfo(fname);

        // copy selected file to ".\\saves\\handin\\......"
        QFile::copy(fname, cwd_win() + "\\saves\\handin\\" + lesson_name + "\\" +
                               finfo.fileName());
    });

    // lesson file button
    auto &lfb = lesson_file_button;
    for (int i = 0; i < 3; i++) {
        lfb[i] = new Button(parent, "", 150, 50);
        lfb[i]->setZValue(1);
    }
    lfb[0]->set_mode(Button::label);
    lfb[0]->set_size(210, 40);
    lfb[0]->set_text("资料");
    lfb[0]->set_point_size(10);
    lfb[0]->setPos(400 + 175 + 245 + 210, 650);
    lfb[0]->set_edge(false);
    lfb[0]->setBrush(Qt::transparent);

    lfb[1]->set_text("电子资料");
    lfb[1]->set_point_size(10);
    lfb[1]->setPos(400 + 175 + 245 + 210 + 30, 650 + 60);
    connect(lfb[1], &Button::clicked, [=] {
        if (selected_lesson_ID == -1) {
            parent->gui->shell->push_msg("[课表]请先选择一门课程.", false);
            return;
        }

        QProcess proc;
        QString path = cwd_win() + "\\saves\\material_digit\\";
        path.append(lesson_ID_to_eve[selected_lesson_ID]->name);

        proc.start("explorer", QStringList(path));
        proc.waitForFinished();
    });

    lfb[2]->set_text("纸质资料");
    lfb[2]->set_point_size(10);
    lfb[2]->setPos(400 + 175 + 245 + 210 + 30, 650 + 60 * 2);
    connect(lfb[2], &Button::clicked, [=] {
        if (selected_lesson_ID == -1) {
            parent->gui->shell->push_msg("[课表]请先选择一门课程.", false);
            return;
        }

        QProcess proc;
        QString path = cwd_win() + "\\saves\\material_paper\\";
        path.append(lesson_ID_to_eve[selected_lesson_ID]->name);

        proc.start("explorer", QStringList(path));
        proc.waitForFinished();
    });

    // lesson_exam_button
    for (int i = 0; i < 5; i++) {
        auto &leb = lesson_exam_button[i];
        leb = new Button(parent, "", 210, 40);
        leb->set_mode(Button::label);
        leb->setZValue(1);
        leb->setPos(400 + 175 + 245 + 210 * 2, 650 + 40 * i);
        leb->set_edge(false);
        leb->setBrush(Qt::transparent);
    }
    lesson_exam_button[0]->set_text("考试");
    lesson_exam_button[0]->set_point_size(10);
}

void TableGUI::update() {
    // lesson chart buttons
    auto le = parent->activ->eves;
    for (auto e : le) {
        if (e->type != "lesson") continue;
        int x, y;
        x = e->begin_time.hour() - 1;
        y = e->begin_time.minute();
        if (y == 8 || y == 13)
            if (e->begin_time.second() == 0) y--;
        if (y <= 10)
            y -= 7;
        else
            y -= 8;

        lesson_eves[y][x] = e;
        lesson_button[y][x]->set_text(e->name);
        lesson_button[y][x]->set_point_size(8);
        lesson_ID_to_eve[e->lesson_ID()] = e;
    }

    // lesson info buttons
    if (selected_lesson_ID == -1) {
        lesson_info_button[0]->set_text("课程");
        for (int i = 1; i < 5; i++) lesson_info_button[i]->set_text("");
    } else {
        Event *e = lesson_ID_to_eve[selected_lesson_ID];
        lesson_info_button[0]->set_text(e->name);
        QString s = "地点:";
        s.append(e->lesson_building()->name());
        lesson_info_button[1]->set_text(s);
        lesson_info_button[2]->set_text(e->lesson_room());
        s = "教师:";
        s.append(e->lesson_teacher());
        lesson_info_button[3]->set_text(s);
        s = "课程群:";
        s.append(e->lesson_group());
        lesson_info_button[4]->set_text(s);
    }
    for (int i = 0; i < 5; i++) lesson_info_button[i]->set_point_size(10);

    // lesson exam buttons
    if (selected_lesson_ID == -1) {
        lesson_exam_button[0]->set_text("考试");
        for (int i = 1; i < 5; i++) lesson_exam_button[i]->set_text("");
    } else {
        Exam *e = nullptr;
        for (auto i : parent->activ->eves)
            if (i->type == "exam") {
                Exam *t = (Exam *)i;
                if (t->lesson_ID == selected_lesson_ID) {
                    e = t;
                    break;
                }
            }
        if (e != nullptr) {
            QString week("一二三四五六日");
            QString s = QString("周%1 %2:%3-%4:%5")
                            .arg(week[e->begin_time.hour() - 1])
                            .arg(e->begin_time.minute())
                            .arg(e->begin_time.second())
                            .arg(e->end_time.minute())
                            .arg(e->end_time.second());
            lesson_exam_button[1]->set_text(s);
            lesson_exam_button[2]->set_text(
                lesson_ID_to_eve[e->lesson_ID]->lesson_room());
        } else {
            lesson_exam_button[0]->set_text("考试");
            lesson_exam_button[1]->set_text("无");
            for (int i = 2; i < 5; i++) lesson_exam_button[i]->set_text("");
        }
    }
    for (int i = 0; i < 5; i++) lesson_exam_button[i]->set_point_size(10);

    // search result
    auto &res = parent->time_table->search_result;
    all_button_lesson_up();
    for(auto r: res) {
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 7; j++)
                if(lesson_eves[i][j] != nullptr && lesson_eves[i][j]->lesson_ID() == r)
                    lesson_button[i][j]->down();
    }
}

void TableGUI::all_button_lesson_up() {
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 7; j++)
            lesson_button[i][j]->up();
}

bool TableGUI::check_undone_assignments(QString lesson_name) {
    QString fpath = cwd_win() + "\\saves\\assignments\\";
    fpath.append(lesson_name).append("\\");
    int big = 0;

    while (1) {
        big++;
        QString fname = fpath;
        QString t = QString("assign%1.txt").arg(big);
        fname.append(t);
        QFile f(fname);
        if (!f.exists()) break;
    }
    big--;

    fpath = cwd_win() + "\\saves\\handin\\";
    fpath.append(lesson_name).append("\\");
    bool all_done = true;
    for (int i = 1; i <= big; i++) {
        QString fname = fpath;
        QString t = QString("handin%1.txt").arg(i);
        fname.append(t);

        QFile f(fname);
        if (!f.exists()) {
            all_done = false;
            QString m = QString("[作业]作业%1尚未提交.").arg(i);
            parent->gui->shell->push_msg(m, false);
        }
    }
    if (all_done) parent->gui->shell->push_msg("所有作业均已提交.", true);

    return all_done;
}

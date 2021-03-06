#include "map.h"
#include <QDir>

#include <QDebug>
#include <QTextStream>
#include <algorithm>

#include "code/activity/activity.h"
#include "code/activity/event/move.h"
#include "code/gui/gui.h"
#include "code/gui/residant/digit_clock.h"
#include "code/primary.h"
#include "code/tool_lib.h"
#include "code/view/view.h"

Map::Map(Primary *p) : QObject() {
    parent = p;
    set_campus(0);

    // buildings init
    // building(s) are added to scene in map_gui

    // current_building
    QFile current_building_dat(cwd() + "/saves/current_building.dat");
    if (!current_building_dat.exists()) {
        current_building_dat.open(QIODevice::ReadWrite | QIODevice::Text);
        current_building_dat.write("0");
        current_building_dat.close();
    }
    if (current_building_dat.open(QFile::ReadOnly)) {
        QString t;
        // discard comment
        while (1) {
            t = current_building_dat.readLine();
            if (t[0] != '#' && t[0] != '\n') break;
        }
        // read current_building
        if (t.contains("\r\n"))
            t.chop(2);
        else
            t.chop(1);
        current_building = t.toInt();
        qDebug("current_building: %d", current_building);
        current_building_dat.close();
    } else
        panic(
            "Map: fail to open current_building.dat, please check working "
            "directory.");

    // buildings
    QFile buildings_dat(cwd() + "/saves/buildings.dat");
    if (buildings_dat.open(QFile::ReadOnly)) {
        while (!buildings_dat.atEnd()) {
            auto b = new Building(parent, buildings_dat);
            buildings.push_back(b);
            if (b->campus() != current_campus) b->hide();
        }

        buildings_dat.close();
    } else
        panic(
            "Map: fail to open building.dat, please check working directory.");

    // use buildings[i] for Building with ID == i
    std::sort(
        buildings.begin(), buildings.end(),
        [](Building *a, Building *b) -> bool { return a->ID() < b->ID(); });

    // paths
    QFile paths_dat(cwd() + "/saves/paths.dat");
    if (paths_dat.open(QFile::ReadOnly)) {
        QString t;

        while (!paths_dat.atEnd()) {
            // discard comment
            while (1) {
                t = paths_dat.readLine();
                if (t[0] != '#' && t[0] != '\n') break;
            }
            auto p1 = new Path(parent, t, buildings, false);
            auto p2 = new Path(parent, t, buildings, true);
            p1->set_reverse_path(p2);
            p2->set_reverse_path(p1);
            paths.push_back(p1);
            paths.push_back(p2);
            if (p1->campus() != current_campus) p1->hide(), p2->hide();
        }

        paths_dat.close();
    } else
        panic("Map: fail to open paths.dat, please check working directory.");

    pix = new QGraphicsPixmapItem;
    QPixmap *pixmap = new QPixmap(":/img/building/my_location.png");
    pix->setPixmap(pixmap->scaled(30, 30));
}

Map::~Map() {
    qDebug("Map deleted");
    QFile current_building_dat(cwd() + "/saves/current_building.dat");
    if (current_building_dat.open(QFile::WriteOnly | QFile::Truncate)) {
        char buf[10];
        sprintf(buf, "%d\n", current_building);
        current_building_dat.write(buf);
        current_building_dat.close();
    } else
        panic(
            "Map: fail to open current_building.dat, please check working "
            "directory.");
}

void Map::set_campus(int n) {
    if (n != 0 && n != 1) panic("Map: no such campus.");
    current_campus = n;
    // emit core_msg(QString("(test core)???????????????%1.").arg(char(n + 'A')), true);
}

// n = 0 -> time        n = 1 -> distance
void Map::move_to(Building *dest, int n, bool bike) {
    // calculate
    /* ????????????: Map::buildings
     * ?????????: Map::paths
     * ?????????: buildings[current_building]
     * ?????????: dest
     * ?????????????????????out_paths??????,????????????????????????????????????
     *
     * ??????:??????????????????????????????????????????????????????,??????????????????
     * ????????????????????????QVector<Path*> solution_paths,
     */

    bool visit[20000] = {0};
    long long dis[20000];
    int num = buildings.size();
    long long minn;
    int destina = dest->ID();
    int curr = current_building;
    QVector<Path *> path;
    QVector<Path *> solution_path;
    QVector<Path *> temp_path;
    QVector<QVector<Path *>> record_path(num);

    for (int i = 0; i < num; i++) {
        dis[i] = 0x7FFFFFFF;
        visit[i] = false;
    }
    dis[curr] = 0;

    if (n == 1 && bike == 0) {
        while (!visit[destina] && !visit[curr]) {
            visit[curr] = true;
            path = buildings[curr]->out_paths();
            int path_size = path.size();
            for (int i = 0; i < path_size; i++) {
                if (!visit[path[i]->to_building()->ID()] &&
                    dis[path[i]->to_building()->ID()] >
                        dis[curr] + path[i]->distance()) {
                    dis[path[i]->to_building()->ID()] =
                        dis[curr] + path[i]->distance();
                    if (!temp_path.isEmpty()) {
                        temp_path.clear();
                    }
                    temp_path.push_back(path[i]);
                    record_path[path[i]->to_building()->ID()] =
                        record_path[curr];
                    record_path[path[i]->to_building()->ID()].append(temp_path);
                }
            }

            minn = 0x7FFFFFFF;

            for (int i = 0; i < num; i++) {
                if (!visit[i] && minn > dis[i]) {
                    minn = dis[i];
                    curr = i;
                }
            }
        }

        if (dis[dest->ID()] < 0x7FFFFFFF) {
            // current_building = dest->ID();
            solution_path = record_path[dest->ID()];
        }
    } else if (n == 1 && bike == 1) {
        while (!visit[destina] && !visit[curr]) {
            visit[curr] = true;
            path = buildings[curr]->out_paths();
            int path_size = path.size();
            for (int i = 0; i < path_size; i++) {
                if (path[i]->can_bike() == false) continue;

                if (!visit[path[i]->to_building()->ID()] &&
                    dis[path[i]->to_building()->ID()] >
                        dis[curr] + path[i]->distance()) {
                    dis[path[i]->to_building()->ID()] =
                        dis[curr] + path[i]->distance();
                    if (!temp_path.isEmpty()) {
                        temp_path.clear();
                    }
                    temp_path.push_back(path[i]);
                    record_path[path[i]->to_building()->ID()] =
                        record_path[curr];
                    record_path[path[i]->to_building()->ID()].append(temp_path);
                }
            }

            minn = 0x7FFFFFFF;

            for (int i = 0; i < num; i++) {
                if (!visit[i] && minn > dis[i]) {
                    minn = dis[i];
                    curr = i;
                }
            }
        }

        if (dis[dest->ID()] < 0x7FFFFFFF) {
            // current_building = dest->ID();
            solution_path = record_path[dest->ID()];
        }
    } else if (n == 0 && bike == 0) {
        while (!visit[destina] && !visit[curr]) {
            visit[curr] = true;
            path = buildings[curr]->out_paths();
            int path_size = path.size();
            for (int i = 0; i < path_size; i++) {
                if (!visit[path[i]->to_building()->ID()] &&
                    dis[path[i]->to_building()->ID()] >
                        dis[curr] + path[i]->time_cost()) {
                    dis[path[i]->to_building()->ID()] =
                        dis[curr] + path[i]->time_cost();
                    if (!temp_path.isEmpty()) {
                        temp_path.clear();
                    }
                    temp_path.push_back(path[i]);
                    record_path[path[i]->to_building()->ID()] =
                        record_path[curr];
                    record_path[path[i]->to_building()->ID()].append(temp_path);
                }
            }

            minn = 0x7FFFFFFF;

            for (int i = 0; i < num; i++) {
                if (!visit[i] && minn > dis[i]) {
                    minn = dis[i];
                    curr = i;
                }
            }
        }

        if (dis[dest->ID()] < 0x7FFFFFFF) {
            // current_building = dest->ID();
            solution_path = record_path[dest->ID()];
        }
    } else if (n == 0 && bike == 1) {
        while (!visit[destina] && !visit[curr]) {
            visit[curr] = true;
            path = buildings[curr]->out_paths();
            int path_size = path.size();
            for (int i = 0; i < path_size; i++) {
                if (path[i]->can_bike() == false) continue;

                if (!visit[path[i]->to_building()->ID()] &&
                    dis[path[i]->to_building()->ID()] >
                        dis[curr] + path[i]->time_cost()) {
                    dis[path[i]->to_building()->ID()] =
                        dis[curr] + path[i]->time_cost();
                    if (!temp_path.isEmpty()) {
                        temp_path.clear();
                    }
                    temp_path.push_back(path[i]);
                    record_path[path[i]->to_building()->ID()] =
                        record_path[curr];
                    record_path[path[i]->to_building()->ID()].append(temp_path);
                }
            }

            minn = 0x7FFFFFFF;

            for (int i = 0; i < num; i++) {
                if (!visit[i] && minn > dis[i]) {
                    minn = dis[i];
                    curr = i;
                }
            }
        }

        if (dis[dest->ID()] < 0x7FFFFFFF) {
            // current_building = dest->ID();
            solution_path = record_path[dest->ID()];
        }
    } else
        panic("Map: move_to");
    
    if(solution_path.empty()) {
        // no path found
        emit core_msg("???????????????.", true);
        return;
    }

    // highlight paths
    for (auto i : solution_path) {
        i->set_highlight(true);
        i->rever_path()->set_highlight(true);
    }

    // create move event
    QTime ct = parent->gui->digit_clock->current_time();
    EventInfo ei = {"move", "??????",         add_minute(ct, 1),
                    ct,     QTime(0, 0, 0), 0};  // end time is set by Move()
    parent->activ->add_event(new Move(parent, ei, solution_path, n));

    // shell message
    QString stratage;
    if (n == 0)
        stratage = "??????????????????";
    else if (n == 1)
        stratage = "??????????????????";
    QString bi;
    if (bike)
        bi = "????????????";
    else
        bi = "??????";
    QString des(dest->name());

    QString m("??????");
    m.append(stratage).append(bi).append("??????").append(des).append(".");
    emit core_msg(m, true);
}

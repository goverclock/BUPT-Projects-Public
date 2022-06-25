#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QObject>
#include <QLineEdit>

class Primary;
enum SCENE { map, activ, table };

class View : public QGraphicsView {
    Q_OBJECT
   public:
#define APP_WIDTH 1550
#define APP_HEIGHT 950
    View(Primary *);
    ~View();

    Primary *parent;

    void goto_scene(SCENE);

    // getter
    QGraphicsScene &cur_scene();

    QGraphicsScene *sce_map;
    QGraphicsScene *sce_activ;
    QGraphicsScene *sce_table;

    QLineEdit *search;
   public slots:
    void call_do_search(QString);

   private:
    bool eventFilter(QObject *, QEvent *);
    void keyPressEvent(QKeyEvent *event);
};

static void goto_scene_map(View *);
static void goto_scene_activ(View *);
static void goto_scene_table(View *);

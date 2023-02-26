#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QVector>

class Primary;
class Button;

class Menu : public QObject, public QGraphicsRectItem {
    Q_OBJECT
   public:
    Menu(Primary *p, int w, int h);

    Primary *parent;

    bool is_on();
    void on();
    void off();
    void add_button(Button *);

   private:
    bool _is_on = false;
    QVector<Button *> buttons;
    int width;
    int height_each;  // of each button in the menu

    // void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
};

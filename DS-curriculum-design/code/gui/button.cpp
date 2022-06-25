#include "button.h"

#include "code/tool_lib.h"

Button::Button(Primary *p, QString txt, int w, int h)
    : QObject(), QGraphicsRectItem() {
    parent = p;
    width = w;
    height = h;
    setRect(0, 0, width, height);
    setBrush(Qt::white);
    setAcceptHoverEvents(true);
    set_mode(normal);

    // text position will be changed if call:
    // set_ico
    // set_point_size
    text = new Text(this, txt);
    set_point_size(50);
}

bool Button::is_on() { return _is_on; }
void Button::on(QGraphicsScene &s) {
    s.addItem(this);
    _is_on = true;
}
void Button::off(QGraphicsScene &s) {
    if (is_on()) s.removeItem(this);
    _is_on = false;
}

bool Button::is_down() { return _is_down; }
void Button::down() {
    _is_down = true;
    QColor c;
    c = QColor::fromRgb(214, 216, 217);
    setBrush(c);
}
void Button::up() {
    _is_down = false;
    setBrush(Qt::white);
}

void Button::set_text(QString t) { text->set_text(t); }

// note: calls disconnect()
// always connect event to clicked signal
void Button::set_mode(button_mode m) {
    disconnect();
    switch (m) {
        case normal:
            connect(this, &Button::clicked, &Button::down);
            connect(this, &Button::released, &Button::up);
            break;
        case label:
            break;
        case men:
            connect(this, &Button::hover_enter, &Button::down);
            connect(this, &Button::hover_leave, &Button::up);
            break;
        case swit:
            connect(this, &Button::clicked, &Button::down);
            // connect other events to call up()
            break;
    }
}

void Button::set_edge(bool a) {
    if (!a)
        setPen(QPen(Qt::transparent));
    else
        setPen(QPen(Qt::black));
}

// note: repositions the text
void Button::set_ico(QString path) {
    ico = new QGraphicsPixmapItem(QPixmap(path));
    ico->setParentItem(this);
    text->setPos(center_pos(rect().width() + ico->pixmap().width() / 2,
                            rect().height() * 0.9, text->width(),
                            text->height()));
}

// note: reset the point size
void Button::set_size(int w, int h) {
    width = w;
    height = h;
    setRect(0, 0, w, h);
    set_point_size(50);
}

void Button::set_point_size(int n) {
    text->set_point_size(n);
    text->setPos(center_pos(rect().width(), rect().height(), text->width(),
                            text->height()));
}

void Button::create_menu(int w, int h) {
    if (h == -1) h = height;
    _menu = new Menu(parent, w, h);
    _menu->setPos(x() + width, y());
}

Menu &Button::menu() { return *_menu; }

bool Button::has_menu() { return _menu != nullptr; }

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    emit hover_enter();
    // menu
    if (has_menu()) menu().on();
}
void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    emit hover_leave();
    // menu
    if (!has_menu()) return;
    int ey = event->pos().y(), ex = event->pos().x();
    // on the right edge?
    // note: ex >= width - 1
    if (ey >= 0 && ey <= height && ex >= width - 1) return;
    menu().off();
}
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}
void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    emit released();
}

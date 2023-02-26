#include "menu.h"
#include "code/gui/button.h"
#include "code/primary.h"
#include "code/view/view.h"

Menu::Menu(Primary *p, int w, int h) : QObject(), QGraphicsRectItem() {
    parent = p;
    width = w;
    height_each = h;

    // pos is already set by the button in create_button()
    setRect(0, 0, width, height_each);
    setAcceptHoverEvents(true);
    // nothing can hide a menu
    setZValue(100);
}

bool Menu::is_on() { return _is_on; }
void Menu::on() {
    if(is_on()) return;
    parent->view->cur_scene().addItem(this);
    for(auto i : buttons)
        i->on(parent->view->cur_scene());
    _is_on = true;
}
void Menu::off() {
    if(!is_on()) return;
    parent->view->cur_scene().removeItem(this);
    for(auto i :buttons) {
        if(i->has_menu()) i->menu().off();
        i->off(parent->view->cur_scene());
    }
    _is_on = false;
}

// buttons in a menu should NOT be added to a scene in GUI::all_xxx_gui_on()
// but should be initialized in GUI::all_xxx_gui_on()
// automatically set nbutton: setRect, setPos, set_edge, set_mode
void Menu::add_button(Button *nbutton) {
    buttons.push_back(nbutton);
    setRect(0, 0, width, height_each * buttons.size());

    nbutton->set_size(width, height_each);
    nbutton->setRect(1, 1, width - 1, height_each - 1);
    nbutton->setPos(x(), y() + height_each * (buttons.size() - 1));
    nbutton->set_edge(false);
    nbutton->set_point_size(height_each / 3);
    nbutton->setZValue(zValue());

}

// removed feature because bug exists
// void Menu::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
//     int ey = event->pos().y(), ex = event->pos().x();
//     // on the right edge?
//     if (ey >= 0 && ey <= height_each && ex >= width - 1) return;
//     off();
// }

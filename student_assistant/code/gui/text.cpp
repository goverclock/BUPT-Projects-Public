#include "text.h"

#include <QFontDatabase>

Text::Text(QGraphicsItem* p, const QString& txt) {
    setParentItem(p);
    parent = p;

    // int font_id =
    //     QFontDatabase::addApplicationFont(":/font/IPix_Chinese_Pixel_Font.ttf");
    // family = QFontDatabase::applicationFontFamilies(font_id).at(0);
    family = "times";
    point_size = 20;
    weight = QFont::Bold;

    QFont fnt(family, point_size, weight);
    setFont(fnt);
    setPlainText(txt);
}

// return the width of the QGraphicsTextItem
int Text::width() {
    int ch = 0, oth = 0;
    for (auto i : toPlainText()) {
        QString s(":->");
        if (i.isNumber() || s.contains(i))
            oth++;
        else
            ch++;
    }

    return (point_size + 3) * (ch * 2 + oth);
    // return (point_size + 3) * 2 * toPlainText().size();
}

// return the height of the QGraphicsTextItem
int Text::height() { return point_size * 2; }

void Text::set_point_size(int sz) {
    point_size = sz;
    QFont fnt(family, sz, weight);
    setFont(fnt);
}

void Text::set_weight(int we) {
    QFont fnt(family, point_size, we);
    setFont(fnt);
}

void Text::set_text(QString txt) { setPlainText(txt); }

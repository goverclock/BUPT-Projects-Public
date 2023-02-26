#pragma once
#include <QGraphicsTextItem>
#include <QString>

class Text : public QGraphicsTextItem {
   public:
    Text(QGraphicsItem *p, const QString& = QString("no text"));

    QGraphicsItem *parent;

    // getters
    int width();
    int height();

    // setters
    void set_point_size(int);
    void set_weight(int);
    void set_text(QString);

   private:
    QString family;
    int point_size;
    int weight;
};

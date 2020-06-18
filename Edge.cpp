#include "Edge.h"
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QDebug>

Edge::Edge(Vertex* p1, Vertex* p2, int weight, QWidget* parent)
    :QLabel(parent),  weight(weight),one(p1), two(p2)
{

}

QPoint Edge::getCenter() const
{
    int x1 = one->getPos().x();
    int x2 = two->getPos().x();
    int y1 = one->getPos(). y();
    int y2 = two->getPos().y();
    return QPoint((x1+x2)/2, (y1+y2)/2);
}

QPoint Edge::getFirst() const
{
    return one->getPos();
}

QPoint Edge::getSecond() const
{
    return two->getPos();
}

Vertex* Edge::getPartner(Vertex* v)
{
    if(v == one){return two;}
    else if(v == two){return one;}
}

Vertex *Edge::getOne()
{
    return one;
}

Vertex *Edge::getTwo()
{
    return two;
}

void Edge::setFirst(Vertex *v)
{
    one = v;
}

void Edge::setSecond(Vertex *v)
{
    two = v;
}

int Edge::getWidth() const
{
    return one->getPos().x()<two->getPos().x()
            ? two->getPos().x() - one->getPos().x() : one->getPos().x() - two->getPos().x();
}

int Edge::getHeight() const
{
    return one->getPos().y()<two->getPos().y()
            ? two->getPos().y() - one->getPos().y() : one->getPos().y() - two->getPos().y();
}

int Edge::getWeight() const
{
    return weight;
}

void Edge::setGeom()
{
    int xTopLeft = one->getPos().x() < two->getPos().x() ? one->getPos().x() : two->getPos().x();
    int yTopLeft = one->getPos().y() < two->getPos().y() ? one->getPos().y() : two->getPos().y();
    int minWidth = this->getWidth() < 7 ? 7 : this->getWidth();
    int minHeight = this->getHeight() < 7 ? 7 : this->getHeight();
    this->setGeometry(xTopLeft, yTopLeft, minWidth, minHeight);
    update();
}

bool Edge::checkPosition(QPoint pos)
{
    int xTopLeft = one->getPos().x() < two->getPos().x() ? one->getPos().x() : two->getPos().x();
    int yTopLeft = one->getPos().y() < two->getPos().y() ? one->getPos().y() : two->getPos().y();
    int xBottomRight = one->getPos().x() > two->getPos().x() ? one->getPos().x() : two->getPos().x();
    int yBottomRight = one->getPos().y() > two->getPos().y() ? one->getPos().y() : two->getPos().y();
    int x = pos.x();
    int y = pos.y();
    return (xTopLeft<=x && x<=xBottomRight)&&(yTopLeft<=y && y<=yBottomRight);
}

bool Edge::includeVertex(Vertex *v)
{
    return (one == v)||(two == v);
}

void Edge::setColor(int num)
{
    status = static_cast<color>(num);
    update();
}

void Edge::paintEvent(QPaintEvent *event)
{
    this->setGeom();
    QPainter p(this);
    QPen pen;
    switch(status)
    {
        case BLACK:
            pen.setColor(Qt::black); break;
        case YELLOW:
            pen.setColor(Qt::yellow); break;
        case BLUE:
            pen.setColor(Qt::blue); break;
        default:
            pen.setColor(Qt::black);
    }
    pen.setWidth(2);
    p.setPen(pen);
    if(one->getPos().y() <= two->getPos().y()){
        if(one->getPos().x()<=two->getPos().x())
            p.drawLine(QPoint(0,0), QPoint(this->getWidth(), this->getHeight()));
        else {
            p.drawLine(QPoint(0, this->getHeight()), QPoint(this->getWidth(), 0));
        }
    }
    else{
        if(one->getPos().x() <= two->getPos().x())
            p.drawLine(QPoint(0, this->getHeight()), QPoint(this->getWidth(), 0));
        else {
            p.drawLine(QPoint(0,0), QPoint(this->getWidth(), this->getHeight()));
        }
    }
    p.end();
    QLabel::paintEvent(event);
    setText(QString::number(weight) + "\n");
    update();
}

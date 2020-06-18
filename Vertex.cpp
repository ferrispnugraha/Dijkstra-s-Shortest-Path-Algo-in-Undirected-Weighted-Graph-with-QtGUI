#include "Vertex.h"
#include <QPainter>
#include <QLabel>
#include <QPoint>
#include <QPaintEvent>
#include <QDebug>
#include <QList>

Vertex::Vertex(int xPos, int yPos, QWidget* parent)
    :  QLabel(parent),xPos(xPos), yPos(yPos)
{

}

Vertex::Vertex(const Vertex &v, QWidget* parent)
    :QLabel(parent), xPos(v.xPos), yPos(v.yPos)
{

}

void Vertex::setPos(QPoint p)
{
    xPos = p.x();
    yPos = p.y();
}

QPoint Vertex::getPos() const
{
    return QPoint(xPos+12, yPos+12);//get the center
}

bool Vertex::checkPosition(QPoint pos)
{
    int diffX = -xPos + pos.x();//x,y pos top left corner
    int diffY = -yPos + pos.y();
    return ((diffX>=0 && diffX<=25) && (diffY>=0 && diffY<=25));
}

void Vertex::printSelf() const
{
    qDebug()<<xPos<<yPos;
}

void Vertex::setColor(int num)
{
    status = static_cast<color>(num);
    update();
}

void Vertex::printNeighbors() const
{
    if(!neighborVertex.empty()){
        QList<Vertex*>::const_iterator it;
        for(it = neighborVertex.begin(); it!= neighborVertex.end(); ++it){
            qDebug()<<(*it)->getPos().x()<<(*it)->getPos().y();
        }
    }
}

bool Vertex::includeNeighbor(Vertex *v) const
{
    QList<Vertex*>::const_iterator ptr;
    for(ptr = neighborVertex.begin(); ptr!=neighborVertex.end(); ++ptr){
        if((*ptr)==v){return true;}
    }
    return false;
}

QList<Vertex *>& Vertex::neighbors()
{
    return neighborVertex;
}

void Vertex::removeNeighbor(Vertex *v)
{
    QList<Vertex*>::iterator it;
    for(it = neighborVertex.begin(); it != neighborVertex.end(); ++it){
        if(*it == v){
            neighborVertex.erase(it);
            break;
        }
    }
}

int Vertex::numOfNeighbors() const
{
    return neighborVertex.size();
}

int Vertex::getWeight() const
{
    return weight;
}

void Vertex::setWeight(int w)
{
    weight = w;
    update();
}

int Vertex::getId() const
{
    return id;
}

void Vertex::setId(int num)
{
    id = num;
}

Vertex* Vertex::getPrevious()
{
    return prev;
}

void Vertex::setPrevious(Vertex *v)
{
    prev = v;
}

void Vertex::mouseMoveEvent(QMouseEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);

    int dx = e->pos().x() - 12; //e->pos() relative to top left
    int dy = e->pos().y() - 12;
    int new_x = this->x() + dx; //new position of top left
    int new_y = this->y() + dy;

    move(new_x, new_y);         //move automatically updates x() and y()
    xPos = new_x;               //update the value after move
    yPos = new_y;
}


void Vertex::paintEvent(QPaintEvent* event){
    QPainter p(this);
    p.setBrush(Qt::white);
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
    p.drawEllipse(QRect(0,0, width(), height()));
    p.end();
    QLabel::paintEvent(event);
    if(weight<2000){
        setText(QString::number(weight));
    }
    else{
        setText(" ");
    }
}

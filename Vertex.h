#ifndef VERTEX_H
#define VERTEX_H

#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QList>

class Vertex : public QLabel
{
    Q_OBJECT
public:
    enum color{BLACK, YELLOW, BLUE};

    explicit Vertex(int xPos, int yPos, QWidget* parent = nullptr);
    Vertex(const Vertex& v, QWidget* parent = nullptr);

    void setPos(QPoint);
    QPoint getPos() const;
    bool checkPosition(QPoint pos);
    void printSelf() const;
    void setColor(int);

    void printNeighbors() const;
    bool includeNeighbor(Vertex*) const;
    QList<Vertex*>& neighbors();
    void removeNeighbor(Vertex*);
    int numOfNeighbors() const;

    int getWeight() const;
    void setWeight(int);

    int getId() const;
    void setId(int);

    Vertex* getPrevious();
    void setPrevious(Vertex*);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;

protected slots:
    void paintEvent(QPaintEvent* event) override;

private:
    int weight = 2000; //max int value, call this "infinite"
    int xPos;
    int yPos;
    color status = BLACK;
    int id;             //for hash

    //for adjacency List, QVector<Vertex*>, where in each Vertex we have QList<Vertex*>
    QList<Vertex*> neighborVertex;

    Vertex* prev = nullptr;
};

#endif // VERTEX_H

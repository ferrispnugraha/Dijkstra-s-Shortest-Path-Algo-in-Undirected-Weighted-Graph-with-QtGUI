#ifndef EDGE_H
#define EDGE_H

#include <QLabel>
#include "Vertex.h"

class Edge : public QLabel
{
    Q_OBJECT
public:
    enum color{BLACK, YELLOW, BLUE};
    explicit Edge(Vertex* p1, Vertex* p2, int weight, QWidget* parent = nullptr);

    QPoint getCenter() const;
    QPoint getFirst() const;
    QPoint getSecond() const;
    Vertex* getPartner(Vertex*);
    Vertex* getOne();
    Vertex* getTwo();

    void setFirst(Vertex*);
    void setSecond(Vertex*);


    int getWidth() const;
    int getHeight() const;
    int getWeight() const;

    void setGeom();

    bool checkPosition(QPoint pos);
    bool includeVertex(Vertex*);

    void setColor(int);

protected slots:
    void paintEvent(QPaintEvent* event) override;

private:
    int weight;
    Vertex* one;
    Vertex* two;
    color status = BLACK;
};

#endif // EDGE_H

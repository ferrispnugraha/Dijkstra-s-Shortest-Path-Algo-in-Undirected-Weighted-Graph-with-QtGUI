#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <QVector>

#include "Vertex.h"
#include "Edge.h"

class AdjacencyList
{
public:
    AdjacencyList();
    int size() const;
    void printout() const;

    bool foundVertex(Vertex*) const;
    bool foundVertex(QPoint pos) const;
    Vertex* vertexLocated(QPoint pos) const;

    bool foundEdge(Edge*) const;
    bool foundEdge(QPoint pos) const;
    Edge* edgeLocated(QPoint pos) const;
    Edge* edgeLocated(Vertex*, Vertex*);

    void insertVertex(Vertex*);
    void removeVertex(Vertex*);
    void removeVertex(QPoint pos);

    void insertEdge(Edge*);
    void removeEdge(Edge*);
    int getEdgeWeight(Vertex* , Vertex*);

    void initialize();//for random generate
    Vertex* indexVertex(int);
    int getIndexVertex(Vertex*);

private:
    QVector<Vertex*> allVertex;
    QVector<Edge*> allEdge;
};

#endif // ADJACENCYLIST_H

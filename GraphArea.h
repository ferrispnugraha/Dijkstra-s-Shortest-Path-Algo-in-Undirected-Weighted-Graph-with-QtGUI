#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QMouseEvent>
#include <QLabel>
#include <QPoint>
#include <QVector>
#include <QStack>

#include "Vertex.h"
#include "Edge.h"
#include "AdjacencyList.h"
#include "HashTable.h"
#include "AVLTree.h"
#include "RedBlackTree.h"

class GraphArea : public QLabel
{
    Q_OBJECT
public:
    GraphArea(QWidget* parent = nullptr);

    void setDrawingVertex(bool);
    void setDrawingEdge(bool);
    void setCalculating(bool);
    void setShowing(bool);
    void setShowingRedBlack(bool);

    bool getDrawingVertex();
    bool getDrawingEdge();
    bool getCalculating();
    bool getShowing();
    bool getShowingRedBlack();

    void setUnvisited(Vertex*);
    void implementDijkstra(Vertex*);
    void implementDijkstraRedBlack(Vertex*); //red-black tree
    void displayPath();
    void clearPath();

protected:
    void mousePressEvent(QMouseEvent* event);

signals:
    void drawVertexClicked(QPoint);
    void deleteComponent(QPoint);
    void drawEdgeClicked(QPoint);

    void startCalculating(QPoint);
    void showingPath(QPoint);
    void showingPathRedBlack(QPoint);

private:
    bool isDrawingVertex = false;
    bool isDrawingEdge = false;
    bool isCalculating = false;
    bool isShowing = false;
    bool isShowingRedBlack = false;
    bool canDelete = true;


public:
    AdjacencyList adjacencyList;
    HashTable unvisitedVertices;
    AVLTree<Vertex*> priorityQueue;
    RedBlackTree priorityQueueRedBlack;

private:
    Vertex* source = nullptr;
    Vertex* goal = nullptr;

    QStack<Edge*> shortestPath;
};

#endif // GRAPHAREA_H

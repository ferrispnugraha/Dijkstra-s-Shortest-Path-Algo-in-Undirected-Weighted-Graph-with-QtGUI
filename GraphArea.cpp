#include "GraphArea.h"
#include <QMouseEvent>
#include "Vertex.h"
#include <QDebug>
#include <QMessageBox>
#include "AVLTree.h"
#include "AVLTree.cpp"

GraphArea::GraphArea(QWidget* parent)
    : QLabel(parent)
{

}

void GraphArea::setDrawingVertex(bool ans)
{
    isDrawingVertex = ans;
}

void GraphArea::setDrawingEdge(bool ans)
{
    isDrawingEdge = ans;
}

void GraphArea::setCalculating(bool ans)
{
    isCalculating = ans;
}

void GraphArea::setShowing(bool ans)
{
    isShowing = ans;
}

void GraphArea::setShowingRedBlack(bool ans)
{
    isShowingRedBlack = ans;
}

bool GraphArea::getDrawingVertex()
{
    return isDrawingVertex;
}

bool GraphArea::getDrawingEdge()
{
    return isDrawingEdge;
}

bool GraphArea::getCalculating()
{
    return isCalculating;
}

bool GraphArea::getShowing()
{
    return isShowing;
}

bool GraphArea::getShowingRedBlack()
{
    return isShowingRedBlack;
}

void GraphArea::setUnvisited(Vertex *start)
{
    source = start;
    unvisitedVertices = HashTable(source, adjacencyList);
    canDelete = false;  //may not delete
}

void GraphArea::implementDijkstra(Vertex* destination)
{
    goal = destination;
    //initialize priority queue
    priorityQueue.insert(source);

    while(!priorityQueue.isEmpty()){
        //define current vertex to process, remove from unvisited
        Vertex* current = priorityQueue.findMinimum();

        unvisitedVertices.removeFromHash(current->getId());

        //find the current's neighbors
        QList<Vertex*> currentNeighbor = current->neighbors();
        for(int i=0; i<current->numOfNeighbors(); ++i){
            Vertex* partner = currentNeighbor[i];

            if(unvisitedVertices.inHash(partner)){
                Edge* edgeBetween = adjacencyList.edgeLocated(current, partner);

                //change their weights if necessary
                //if change weight, change the prev vertex
                if(partner->getWeight() >
                        current->getWeight() + edgeBetween->getWeight())
                {
                    partner->setWeight(current->getWeight() + edgeBetween->getWeight());
                    partner->setPrevious(current);
                }
                //insert each into priority queue
                //if not in there yet
                if(!priorityQueue.contain(partner)){
                    priorityQueue.insert(partner);
                }
            }
        }
        //remove the previous minimum from priority_queue
        priorityQueue.remove(current);
//        priorityQueue.removeMinimum();

        //loop to find the next minimum, set as "current"
        //reloop the same
    }
    displayPath();
}

void GraphArea::implementDijkstraRedBlack(Vertex *destination)
{
    goal = destination;
    //initialize priority queue
    priorityQueueRedBlack.insert(source);

    qDebug()<<"-----------------------";
    priorityQueueRedBlack.printOut();
    qDebug()<<"-----------------------";

    while(!priorityQueueRedBlack.isEmpty()){
        //define current vertex to process, remove from unvisited
        Vertex* current = priorityQueueRedBlack.findMinimum();

        unvisitedVertices.removeFromHash(current->getId());

        //find the current's neighbors
        QList<Vertex*> currentNeighbor = current->neighbors();
        for(int i=0; i<current->numOfNeighbors(); ++i){
            Vertex* partner = currentNeighbor[i];

            if(unvisitedVertices.inHash(partner)){
                Edge* edgeBetween = adjacencyList.edgeLocated(current, partner);

                //change their weights if necessary
                //if change weight, change the prev vertex
                if(partner->getWeight() >
                        current->getWeight() + edgeBetween->getWeight())
                {
                    partner->setWeight(current->getWeight() + edgeBetween->getWeight());
                    partner->setPrevious(current);
                }
                //insert each into priority queue
                //if not in there yet
                if(!priorityQueueRedBlack.contain(partner)){
                    priorityQueueRedBlack.insert(partner);

                    qDebug()<<"-----------------------";
                    priorityQueueRedBlack.printOut();
                    qDebug()<<"-----------------------";
                }
            }
        }
        //remove the previous minimum from priority_queue
//        priorityQueueRedBlack.remove(current);
        priorityQueueRedBlack.removeMinimum();

        qDebug()<<"-----------------------";
        priorityQueueRedBlack.printOut();
        qDebug()<<"-----------------------";
        //loop to find the next minimum, set as "current"
        //reloop the same
    }
    displayPath();
}

void GraphArea::displayPath()
{
    if(goal->getPrevious() == nullptr){   //cannot track goal
        QMessageBox::about(this, "Path Not Found", "Please try with other nodes");
    }
    else{//goal tracked
        for(Vertex* i = goal; i->getPrevious()!=nullptr; i = i->getPrevious()){
            Edge* path = adjacencyList.edgeLocated(i, i->getPrevious());
            path->setColor(2);//blue enum
            shortestPath.push(path);
        }
    }
}

void GraphArea::clearPath()
{
    while(!shortestPath.isEmpty()){
        Edge* e = shortestPath.top();
        e->setColor(0); //path return to black
        shortestPath.pop();
    }
    if(source != nullptr){
        source->setColor(0);
        source = nullptr;
    }
    if(goal != nullptr){
        goal->setColor(0);
        goal = nullptr;
    }
    //priority queue already empty
    //clear hashtable, and also set previous to nullptr for all vertex
    unvisitedVertices.initialize();
    unvisitedVertices.printHashTable();
    canDelete = true;
}

void GraphArea::mousePressEvent(QMouseEvent *event)
{
    QPoint mousePos = event->pos();
    if(event->button() == Qt::LeftButton){
        if(isDrawingVertex){
            emit drawVertexClicked(mousePos);
        }
        if(isDrawingEdge){
            emit drawEdgeClicked(mousePos);
        }
        else if(isCalculating){
            emit startCalculating(mousePos);
            isCalculating = false;
        }
        else if(isShowing){
            emit showingPath(mousePos);
        }
        else if(isShowingRedBlack){
            emit showingPathRedBlack(mousePos);
        }
    }
    if(!isDrawingVertex && !isDrawingEdge && canDelete && event->button() == Qt::RightButton){
        emit deleteComponent(mousePos);
    }
}

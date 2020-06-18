#include "AdjacencyList.h"
#include <QDebug>

AdjacencyList::AdjacencyList()
{

}

int AdjacencyList::size() const
{
    return allVertex.size();
}

void AdjacencyList::printout() const
{
    QVector<Vertex*>::const_iterator it;
    for(it = allVertex.begin(); it != allVertex.end(); ++it){
        qDebug()<<(*it)->getId();
    }
    QVector<Edge*>::const_iterator itr;
    for(itr = allEdge.begin(); itr!=allEdge.end(); ++itr){
        qDebug()<<(*itr)->getOne()->getId()<<(*itr)->getTwo()->getId();
    }
}

bool AdjacencyList::foundVertex(Vertex *v) const
{
    QVector<Vertex*>::const_iterator it;
    for(it = allVertex.begin(); it != allVertex.end(); ++it){
        if((*it) == v)return true;
    }
    return false;
}

bool AdjacencyList::foundVertex(QPoint pos) const
{//overloaded function
    QVector<Vertex*>::const_iterator it;
    for(it = allVertex.begin(); it != allVertex.end(); ++it){
        if((*it)->checkPosition(pos))return true;
    }
    return false;
}

Vertex *AdjacencyList::vertexLocated(QPoint pos) const
{
    QVector<Vertex*>::const_iterator it;
    for(it = allVertex.begin(); it != allVertex.end(); ++it){
        if((*it)->checkPosition(pos))
            return (*it);
    }
    return nullptr;
}

bool AdjacencyList::foundEdge(Edge *e) const
{
    QVector<Edge*>::const_iterator it;
    for(it = allEdge.begin(); it != allEdge.end(); ++it){
        if((*it) == e)return true;
    }
    return false;
}

bool AdjacencyList::foundEdge(QPoint pos) const
{//overloaded
    QVector<Edge*>::const_iterator it;
    for(it = allEdge.begin(); it != allEdge.end(); ++it){
        if((*it)->checkPosition(pos))return true;
    }
    return false;
}

Edge *AdjacencyList::edgeLocated(QPoint pos) const
{
    QVector<Edge*>::const_iterator it;
    for(it = allEdge.begin(); it != allEdge.end(); ++it){
        if((*it)->checkPosition(pos))
            return (*it);
    }
    return nullptr;
}

Edge *AdjacencyList::edgeLocated(Vertex *v1, Vertex *v2)
{
    QVector<Edge*>::const_iterator it;
    for(it = allEdge.begin(); it != allEdge.end(); ++it){
        if(((*it)->getOne() == v1  && (*it)->getTwo()==v2) ||
                ((*it)->getOne() == v2 && (*it)->getTwo() == v1))
            return (*it);
    }
    return nullptr;
}

void AdjacencyList::insertVertex(Vertex *v)
{
    //assume vertex without edge directly enters the system
    allVertex.push_back(v);
}

void AdjacencyList::removeVertex(Vertex *v)
{
    //delete edges that include v

    QVector<Edge*>::iterator it;
    for(it = allEdge.begin(); it != allEdge.end(); ++it){
        if((*it)->includeVertex(v)){
            removeEdge(*it);
            --it;           //remember erase shifts
        }
    }

    //then neighbor pops v
    QList<Vertex*> neighbors = v->neighbors();
    for(int i = 0; i<neighbors.size(); ++i){
        Vertex* vertex = neighbors[i];
        if(vertex!=nullptr){
            vertex->removeNeighbor(v);
        }
    }
    neighbors.clear();
    //destroy v, don't forget to deallocate used memory
    QVector<Vertex*>::iterator itv;
    for(itv = allVertex.begin(); itv!=allVertex.end(); ++itv){
        if((*itv)==v){
            allVertex.erase(itv);
            --itv;
            break;
        }
    }
    delete v;
    v = nullptr;
}

void AdjacencyList::insertEdge(Edge *e)
{
    //assume right after MainWindow::addEdge
    allEdge.push_back(e);
    e->getOne()->neighbors().push_back(e->getTwo());
    e->getTwo()->neighbors().push_back(e->getOne());
    //qDebug()<<e->getOne()->includeNeighbor(e->getTwo());
    //edge already store weight
}

void AdjacencyList::removeEdge(Edge *e)
{
    e->setFirst(nullptr);
    e->setSecond(nullptr);
    QVector<Edge*>::iterator it;
    for(it = allEdge.begin(); it != allEdge.end(); ++it){
        if(*it == e){
            allEdge.erase(it);
            --it;
            delete e;
            e = nullptr;
            break;
        }
    }
}

int AdjacencyList::getEdgeWeight(Vertex *v1, Vertex *v2)
{
    QVector<Edge*>::iterator it;
    for(it = allEdge.begin(); it != allEdge.end(); ++it){
        if(((*it)->getOne() == v1 && (*it)->getTwo() == v2) ||
                ((*it)->getTwo() == v2 && (*it)->getOne() == v1)){
            return (*it)->getWeight();
        }
    }
}

void AdjacencyList::initialize()
{
    //idea: repeatedly calls removeVertex(*v)
    QVector<Vertex*>::iterator it;
    for(it = allVertex.begin(); it != allVertex.end(); ++it){
        removeVertex(*it);
        --it;
    }
}

Vertex *AdjacencyList::indexVertex(int i)
{
    return allVertex[i];
}

int AdjacencyList::getIndexVertex(Vertex *v)
{
    int ans = 1001;//dummy value
    for(int i = 0; i<allVertex.size(); ++i){
        if(allVertex[i] == v){
            ans = i;
        }
    }
    return ans;
}

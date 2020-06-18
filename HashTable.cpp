#include "HashTable.h"
#include <QVector>
#include <QDebug>

HashTable::HashTable()
{

}

HashTable::HashTable(Vertex *source, AdjacencyList adj)
    :table(adj.size(), nullptr), keys(adj.size(), 0)
{
    size = adj.size();
    sourceIndex = source->getId();
    for(int i = 0; i<adj.size(); ++i){
        //insert vertex* to table
        //simultaneously insert its hash index to keys
            table.insert(table.begin() + adj.indexVertex(i)->getId(), adj.indexVertex(i));
            keys.insert(keys.begin() + adj.indexVertex(i)->getId(), hashIndex(adj.indexVertex(i)));
    }
}

HashTable::HashTable(const HashTable &h)
    : size(h.size), sourceIndex(h.sourceIndex)
{
    table.clear(); keys.clear();
    for(int i=0; i<h.keys.size(); ++i){
        keys[i] = h.keys[i];
    }

    for(int j=0; j<h.size; ++j){
        table[j] = h.table[j];
    }
}

const HashTable &HashTable::operator=(const HashTable &h)
{
    if(this != &h){
        table.clear(); keys.clear();
        size = h.size;
        sourceIndex = h.sourceIndex;
        for(int i=0; i<h.keys.size(); ++i){
            table.insert(table.begin() + i, h.table[i]);
            keys.insert(keys.begin() + i, h.keys[i]);
        }
    }
    return (*this);
}

void HashTable::printHashTable()
{
    int counter = size;
    for(int i=0; counter>0; ++i){
        if(table[i] == nullptr){
            continue;
        }
        if(inHash(table[i])){
            qDebug()<<table[i]->getId()<<keys[i];
            --counter;
        }
    }
    //remember, earlier index can also have nullptr
}

int HashTable::hashIndex(Vertex *v)
{
    return (v->getId() % size)+1;
    //size is 1000
    //avoid zero index
}

Vertex *HashTable::accessVertex(int index)
{
    return table[index];
}

void HashTable::removeFromHash(int index)
{
    //Not actual removing, only mark the key of index-th vertex* as 0, means disappear
    keys[index] = 0;
}

int HashTable::getSourceIndex() const
{
    return sourceIndex;
}

bool HashTable::inHash(Vertex *v)
{
    //avoid out of index
    return (keys[v->getId()] == hashIndex(v));
}

void HashTable::initialize()
{
    if(!table.empty()){
        while(!table.empty()){
            Vertex* v = table.back();
            if(v!=nullptr){
                v->setWeight(2000);//back to dummy value of infinity
                v->setPrevious(nullptr);//for clear
            }
            table.pop_back();
            --size;
        }
    }
    keys.clear();
}

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <QVector>
#include "Vertex.h"
#include "AdjacencyList.h"

class HashTable
{
public:
    HashTable();
    HashTable(Vertex* source, AdjacencyList adj);//include inserting
    HashTable(const HashTable& h);
    const HashTable& operator=(const HashTable& h);

    void printHashTable();

    int hashIndex(Vertex*);                      //hash function
    Vertex *accessVertex(int index);             //access vertex
    void removeFromHash(int index);              //remove visited vertex
    int getSourceIndex() const;

    bool inHash(Vertex*);

    void initialize();

private:
    QVector<Vertex*> table;
    QVector<int> keys;
    int size = 1000;//some large number
    int sourceIndex;
};

#endif // HASHTABLE_H

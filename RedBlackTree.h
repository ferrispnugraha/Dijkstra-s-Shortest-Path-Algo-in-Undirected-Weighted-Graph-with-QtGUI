#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#include "Vertex.h"


/*
* Properties of Red-Black Tree:
* 1. Every node is either RED or BLACK
* 2. The root and null leaves are BLACK
* 3. If a node is red, its children should be BLACK
* 4. All paths from a node to null leaves should contain same number of BLACK nodes
*/

class RedBlackTree
{
private:
    enum color{RED, BLACK, DOUBLE_BLACK};

    struct Node{
        Vertex* v;
        int color;
        Node* left, *right, *parent;
        Node():v(nullptr), color(BLACK), left(nullptr), right(nullptr), parent(nullptr){}
        //for the nil "leaves"
        Node(Vertex* x):v(x), color(RED), left(nullptr), right(nullptr), parent(nullptr){}
        //when inserting, default color is RED
        Node& operator=(Node& node)
        {
            this->v = node.v;
            this->color = node.color;
            this->left = node.left;
            this->right = node.right;
            this->parent = node.parent;
            return *this;
        }
    };

    Node null = {};//for the nil "leaves"

    Node* root = &null;
    Node* nil = &null;

    Node* rightSubtree() { return root->right; }
    Node* leftSubtree() { return root->left; }
    const Node* rightSubtree() const { return root->right; }
    const Node* leftSubtree() const { return root->left; }

    void rotateLeft(Node*);
    void rotateRight(Node*);
    void transplant(Node*, Node*);
    Node* search(Vertex*);

    void fixInsertTree(Node*&);//recoloring and rotation after insert
    void fixRemoveTree(Node*&);//recoloring and rotation after remove
    Node* minValueNode(Node*&);
    Node* maxValueNode(Node*&);

    void printOut(Node*, int depth) const;

public:
    RedBlackTree() = default;
    ~RedBlackTree() = default;
    void insert(Vertex*);//see comment in cpp
    void remove(Vertex*);//see comment in cpp
    void removeMinimum();
    bool contain(Vertex*);
    bool isEmpty() const {return (root == nil);}
    Vertex* findMinimum();
    void printOut() const;
};

#endif // REDBLACKTREE_H

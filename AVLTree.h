#ifndef AVLTREE_H
#define AVLTREE_H

#include "Vertex.h"
//#include "Edge.h"

//Adelson-Velskii and Landis Balanced Binary Search Tree
//for Priority Queue
//duplicate key?

template <typename T>
class AVLTree
{
private:
    struct Node
    {
        T vertex;
        int height;
        AVLTree left;                                   // Left subtree is also AVLTree object
        AVLTree right;                                  // Right subtree is also AVLTree object
        Node(const T x) : vertex(x), height(0), left(), right() { }
    };

    Node* root = nullptr;
    AVLTree& rightSubtree() { return root->right; }
    AVLTree& leftSubtree() { return root->left; }
    const AVLTree& rightSubtree() const { return root->right; }
    const AVLTree& leftSubtree() const { return root->left; }

    int height() const;                                 // height of tree
    int balanceFactor() const;                          // balance factor of tree
    void fixingHeight() const;                          // Rectify the height of each node
    void leftRotation();                                // Single left rotation
    void rightRotation();                               // Single right rotation
    void balancing();                                   // AVL balancing
    Node *minValueNode();
public:
    AVLTree() = default;                                // empty AVL tree by default
    ~AVLTree() { delete root; }                         // Recursively delete
    bool isEmpty() const { return root == nullptr; }
    const T findMinimum() const;                       // Find minimum value
    bool contain(const T x) const;                     // Search
    void printOut(int depth = 0) const;                 // Print by rotating -90 degrees
    void insert(const T x);                            // Insert an item in sorted order
    void remove(const T x);                            // Remove an item
    void removeMinimum();
};

//#include "AVLTree.cpp"

#endif // AVLTREE_H

#include "AVLTree.h"
#include <QDebug>

template<typename T>
int AVLTree<T>::height() const
{
    return isEmpty() ? -1 : root->height;
    //height of leaf is -1
}

template<typename T>
int AVLTree<T>::balanceFactor() const
{
    return isEmpty() ? 0
                     : rightSubtree().height() - leftSubtree().height();
}

template<typename T>
void AVLTree<T>::fixingHeight() const
{
    if(!isEmpty())
    {
        int leftHeight = leftSubtree().height();
        int rightHeight = rightSubtree().height();
        root->height = 1 + std::max(leftHeight, rightHeight);
    }
}

template<typename T>
void AVLTree<T>::leftRotation()
{
    Node* node = rightSubtree().root;
    rightSubtree() = node->left;
    node->left = *this;
    fixingHeight();
    this->root = node;
    fixingHeight();
}

template<typename T>
void AVLTree<T>::rightRotation()
{
    Node* node = leftSubtree().root;
    leftSubtree() = node->right;
    node->right = *this;
    fixingHeight();
    this->root = node;
    fixingHeight();
}

template<typename T>
void AVLTree<T>::balancing()
{
    if(isEmpty())
        return;

    fixingHeight();
    int balanceFact = balanceFactor();
    if(balanceFact == 2)
    {
        if(rightSubtree().balanceFactor() < 0){
            rightSubtree().rightRotation();
        }
        return leftRotation();
    }

    else if(balanceFact == -2)
    {
        if(leftSubtree(). balanceFactor() > 0){
            leftSubtree().leftRotation();
        }
        return rightRotation();
    }
}

template<typename T>
typename AVLTree<T>::Node *AVLTree<T>::minValueNode()
{
    AVLTree& leftAVL = leftSubtree();
    if(leftAVL.isEmpty()){
       return root;
    }

    return leftAVL.minValueNode();
}

template<typename T>
const T AVLTree<T>::findMinimum() const
{
    const AVLTree& leftAVL = leftSubtree();
    if(leftAVL.isEmpty()){
        return root->vertex;
    }

    return leftAVL.findMinimum();
}

template<typename T>
bool AVLTree<T>::contain(const T x) const
{
    if (isEmpty())                   // Reach the leaf
        return false;
    else if ((*x).getWeight() >= (*(root->vertex)).getWeight())  // Found the leaf, operator overloading
    {
        if(x == (root->vertex)){return true;}
        return rightSubtree().contain(x);
    }
    else if ((*x).getWeight() < (*(root->vertex)).getWeight())   // Recursion on the left subtree
        return leftSubtree().contain(x);
}

template<typename T>
void AVLTree<T>::printOut(int depth) const
{
    if(isEmpty())
        return;

    root->right.printOut(depth+1);             //right upward

    for(int j = 0; j<depth; ++j){
        qDebug()<<"\t";
    }
    qDebug() << root->vertex->getId() <<endl;

    root->left.printOut(depth + 1);
}

template<typename T>
void AVLTree<T>::insert(const T x)
{
    if(isEmpty())
        root = new Node(x);
    else if((*x).getWeight() < (*(root->vertex)).getWeight())
        leftSubtree().insert(x);
//    else if((*x).getWeight() > (*(root->vertex)).getWeight())
    else if((*x).getWeight() >= (*(root->vertex)).getWeight())
        rightSubtree().insert(x);
    balancing();
}

template<typename T>
void AVLTree<T>::remove(const T x)
{
    if(isEmpty())                 //x not found
        return;
    if((*x).getWeight() < (*(root->vertex)).getWeight())      //operator overloading, left subtree
        leftSubtree().remove(x);
    else if((*x).getWeight() > (*(root->vertex)).getWeight()) //operator overloading, right subtree
        rightSubtree().remove(x);
    else {
//            if(x== root->vertex)                         //found
        {
            AVLTree& leftAVL = leftSubtree();
            AVLTree& rightAVL = rightSubtree();

            if(!leftAVL.isEmpty() && !rightAVL.isEmpty())       //this node has 2 children
            {
                root->vertex = rightAVL.findMinimum();          //find the minimum from rightAVL
                rightAVL.remove(root->vertex);                  //remove it from the right subtree
            }

            else                                                //Node found, either with 0 or 1 child
            {
                Node* target = root;
                *this = leftAVL.isEmpty() ? rightAVL : leftAVL;

                rightAVL.root =  leftAVL.root = nullptr;
                delete target;
            }
        }
//            else{leftSubtree().remove(x);}
    }

    balancing();
}

template<typename T>
void AVLTree<T>::removeMinimum()
{
    Node* target = minValueNode();
    AVLTree& leftAVL = target->left;
    AVLTree& rightAVL = target->right;

//    *(target) = leftAVL.isEmpty() ? rightAVL : leftAVL;

    rightAVL.root =  leftAVL.root = nullptr;
    delete target;

    balancing();
}

template class AVLTree<Vertex*>;

#include "RedBlackTree.h"
#include <QDebug>

void RedBlackTree::rotateLeft(RedBlackTree::Node *ptr)
{
    Node* child = ptr->right;
    ptr->right = child->left;
    if(child->left != nil){
        child->left->parent = ptr;
    }
    child->parent = ptr->parent;
    if(child->parent == nullptr){
        root = child;
    }
    else if(ptr == ptr->parent->left){
        ptr->parent->left = child;
    }
    else{
        ptr->parent->right = child;
    }
    child->left = ptr;
    ptr->parent = child;
}

void RedBlackTree::rotateRight(RedBlackTree::Node *ptr)
{
    //mirror case of left rotation
    Node* child = ptr->left;
    ptr->left = child->right;
    if(child->right != nil){
        child->right->parent = ptr;
    }
    child->parent = ptr->parent;
    if(child->parent == nullptr){
        root = child;
    }
    else if(ptr == ptr->parent->left){
        ptr->parent->left = child;
    }
    else{
        ptr->parent->right = child;
    }
    child->right = ptr;
    ptr->parent = child;
}

void RedBlackTree::transplant(RedBlackTree::Node *older, RedBlackTree::Node *newer)
{//transplant subtree newer to older, helper function
//    if(older->parent == nil)
    if(older->parent == nullptr)
    {
        root = newer;
    }
    else if(older == older->parent->left){
        older->parent->left = newer;
    }
    else{
        older->parent->right = newer;
    }
    newer->parent = older->parent;
}

RedBlackTree::Node *RedBlackTree::search(Vertex *v1)
{
    Node* current = root;
    while(current != nil && current->v != v1){
        if(current->v->getWeight() > v1->getWeight()){//v1 < current
            current = current->left;
        }
        else if(current->v->getWeight() < v1->getWeight())
        {
            current = current->right;
        }
        else {
            if(current->v == v1){
                return current;
            }
            else current = current->left;//find highest priority
        }
    }
    return current;//if not found, nil
}

bool RedBlackTree::contain(Vertex *v)
{
    return (search(v) != nil);
}

Vertex *RedBlackTree::findMinimum()
{
    return minValueNode(root)->v;
}

void RedBlackTree::printOut() const
{
    printOut(root, 0);
}


/*
* Important cases during insertion:
* For insertion, we check parent and uncle
* 1. current node is root                               => just change the color of root to black
* 2. current node's parent would be black               => don't need to adjust
* 3. current node's parent and uncle is red             => recolor them to black, grandparent becomes red, if it is root
*                                                          ,then readjust to black
* 4. current node's parent is red, but uncle is black   => rotate parent(if zigzag) then
*                                                          rotate grandparent & switch color between them (parent and grandparent)
*/
void RedBlackTree::fixInsertTree(RedBlackTree::Node *&current)
{
    while(current->parent != nullptr && current->parent->color == RED)
    {
        if(current->parent == current->parent->parent->left)//parent is left child
        {
            Node* uncle = current->parent->parent->right;
            if(uncle->color == RED)//case 3
            {
                current->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                current = current->parent->parent; //continue to check upwards
            }
            else                   //case 4
            {
                if(current == current->parent->right)//if zigzag
                {
                    current = current->parent;
                    rotateLeft(current);//rotate parent
                }
                current->parent->color = BLACK;//remember, current is below now
                current->parent->parent->color = RED;
                rotateRight(current->parent->parent);//rotate grandparent
            }
        }
        else//mirror case, parent is right child
        {
            Node* uncle = current->parent->parent->left;
            if(uncle->color == RED)//case 3
            {
                uncle->color = BLACK;
                current->parent->color = BLACK;
                uncle->parent->color = RED;
                current = current->parent->parent; //continue to check upwards
            }
            else                   //case 4
            {
                if(current == current->parent->left)//if zigzag
                {
                    current = current->parent;
                    rotateRight(current);//rotate parent
                }
                current->parent->color = BLACK;//remember, current is below now
                current->parent->parent->color = RED;
                rotateLeft(current->parent->parent);//rotate grandparent
            }
        }
    }
    //for case 1 and 2
    root->color = BLACK;
}

/*
* Important cases during deletion fixation (i.e. deleting black node):
* For deletion, we check siblings
* (For current is the left child of its parent)
* 1. The new BLACK node is new root                    => no problem, minus one black for all available paths, add new one, preserved
* 2. Sibling is RED, and its children are BLACK        => rotate through parent, sibling to BLACK, parent to RED, relabel, continue
* 3. Parent, sibling, sibling's children are all BLACK => sibling to RED, check the parent
* 4. Sibling and its children are all BLACK, but parent is RED => set parent to BLACK, sibling to RED, check the parent
* 5. Sibling is BLACK, left child is RED, right child is BLACK => set sibling to RED, left child to BLACK, right rotate sibling,
*                                                                 continue to case 6
* 6. Sibling is BLACK, right child is RED => set sibling to parent's color, set parent to BLACK, sibling's right child to BLACK,
*                                            left rotate about parent, number of black nodes don't change, can terminate
*/
void RedBlackTree::fixRemoveTree(RedBlackTree::Node *&current)
{
    while(current != root && current->color == BLACK)
    {
        if(current == current->parent->left)//current is left child
        {
            Node* sibling = current->parent->right;
            if(sibling->color == RED)       //case 2
            {
                sibling->color = BLACK;
                current->parent->color = RED;
                rotateLeft(current->parent);//rotate about parent
                sibling = current->parent->right;//change position of pointed sibling
            }
            if(sibling->left->color == BLACK && sibling->right->color == BLACK)//case 3
            {
                sibling->color = RED;
                current = current->parent;
                //then continue the while-loop
            }
            else
            {
                if(sibling->right->color == BLACK) //case 5
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = current->parent->right;//change position of pointed sibling
                }
                //proceed to case 6
                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(current->parent);
                current = root;//terminate
            }

        }
        else //mirror case, current is right child
        {
            Node* sibling =  current->parent->left;
            if(sibling->color == RED)       //case 2
            {
                sibling->color = BLACK;
                current->parent->color = RED;
                rotateRight(current->parent);//rotate about parent
                sibling = current->parent->left;//change position of pointed sibling
            }
            if(sibling->left->color == BLACK && sibling->right->color == BLACK)//case 3
            {
                sibling->color = RED;
                current = current->parent;
                //then continue the while-loop
            }
            else
            {
                if(sibling->left->color == BLACK) //case 5
                {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = current->parent->left;//change position of pointed sibling
                }
                //proceed to case 6
                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(current->parent);
                current = root;//terminate
            }
        }
    }
    //case 1(current == root), case 4 or after case 6 (current->color==RED)
    current->color = BLACK;
}

RedBlackTree::Node *RedBlackTree::minValueNode(RedBlackTree::Node *&root)
{
    Node* current = root;
    //just go left
    while(current->left != nil){
        current = current->left;
    }
    return current; //reach the limit
}

RedBlackTree::Node *RedBlackTree::maxValueNode(RedBlackTree::Node *&root)
{
    //mirror case of minValueNode
    Node* current = root;
    //just go right
    while(current->right != nil){
        current = current->right;
    }
    return current; //reach the limit
}

void RedBlackTree::printOut(RedBlackTree::Node *node, int depth) const
{
    if(node == nil){
        return;
    }

//    qDebug()<<"-----------------------";
    printOut(node->right, depth + 1); //print from right

    for(int j=0 ; j<depth; ++j){
        qDebug()<<"     ";
    }
    qDebug()<<node->v->getId();

    printOut(node->left, depth + 1);
//    qDebug()<<"-----------------------";
}

void RedBlackTree::insert(Vertex *v)
{
    Node* addOne = new Node(v);
    addOne->left = addOne->right = nil;
//    addOne->parent = nullptr;
//    addOne->parent = nil;

//    Node* before = nil;//for record
    Node* before = nullptr;
    Node* current = root;//counter

    while(current!= nil){
        before = current;
        if(v->getWeight() < current->v->getWeight()){
            current = current->left;
        }
        else if(v->getWeight() >= current->v->getWeight()){
            current = current->right;
        }
    }

    addOne->parent = before;

    if(before == nullptr){
        root = addOne;
    }
//    else if(before == before->parent->left){
//        before->parent->left = addOne;
//    }
//    else{
//        before->parent->right = addOne;
//    }
    else if(addOne->v->getWeight() < before->v->getWeight()){
        before->left = addOne;
    }
    else{
        before->right = addOne;
    }

    if(addOne->parent == nullptr){//root
        addOne->color = BLACK;
        return;
    }

    if(addOne->parent->parent == nullptr){
        return;
    }

    fixInsertTree(addOne);
}

//deal with a one-child deletion first
//if two-child, find minimum from right subtree, replace, then deal with deletion
//use transplant helper
//if delete black node, continue to fixRemoveTree (recoloring, rotation etc.)
void RedBlackTree::remove(Vertex *v)
{
    Node* toRemove = search(v);
    if(toRemove == nil){
        return;
    }

    Node* current = toRemove;
    int originalColor = current->color;

    Node* child;
    //one-child deletion
    if(toRemove->left == nil){
        child = toRemove->right;
        transplant(toRemove, toRemove->right);//deal with parents
    }
    else if(toRemove->right == nil){
        child = toRemove->left;
        transplant(toRemove, toRemove->left);
    }
    else//two-child deletion
    {
        current = minValueNode(toRemove->right);//from right subtree
        originalColor = current->color;//color to be removed
        child = current->right;
        if(current->parent == toRemove){
            child->parent = current;
        }
        else{
            transplant(current, current->right);
            current->right = toRemove->right;//minValueNode goes upward
            current->right->parent = current;
        }
        transplant(toRemove, current);
        current->left = toRemove->left;
        current->left->parent = current;
        current->color = toRemove->color;
    }

    if(originalColor == BLACK)//continue to fix tree from child
    {
        fixRemoveTree(child);
    }
    delete toRemove;
    return;
}

void RedBlackTree::removeMinimum()
{
    Node* toRemove = minValueNode(root);
    if(toRemove == nil){
        return;
    }

    Node* current = toRemove;
    int originalColor = current->color;

    Node* child;
    //one-child deletion
    if(toRemove->left == nil){
        child = toRemove->right;
        transplant(toRemove, toRemove->right);//deal with parents
    }
    else if(toRemove->right == nil){
        child = toRemove->left;
        transplant(toRemove, toRemove->left);
    }
    else//two-child deletion
    {
        current = minValueNode(toRemove->right);//from right subtree
        originalColor = current->color;//color to be removed
        child = current->right;
        if(current->parent == toRemove){
            child->parent = current;
        }
        else{
            transplant(current, current->right);
            current->right = toRemove->right;//minValueNode goes upward
            current->right->parent = current;
        }
        transplant(toRemove, current);
        current->left = toRemove->left;
        current->left->parent = current;
        current->color = toRemove->color;
    }

    if(originalColor == BLACK)//continue to fix tree from child
    {
        fixRemoveTree(child);
    }
    delete toRemove;
    return;
}


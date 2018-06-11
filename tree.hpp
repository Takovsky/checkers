#ifndef TREE_HPP
#define TREE_HPP

#include "Pawn.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

struct Node
{
    //    std::vector<sf::Vector2i> validFields;
    int value;
    bool isMax;
    std::vector<Pawn> pawns;
    Node *parent;
    std::vector<Node *> sons;
    Node() : value(0) {}
    ~Node()
    {
        for(auto it = sons.begin(); it != sons.end(); it++)
            delete *it;
        sons.clear();
    }
};

struct Tree
{
    Tree() : root(NULL) {}
    ~Tree() { delete root; }
    Node *root;
    int size(Node *root);
    int leaves(Node *root);
    int height(Node *root);
    //    void inOrderWalk(Node *root);
    //    void preOrderWalk(Node *root);
    //    void postOrderWalk(Node *root);

    void setRoot(std::vector<Pawn> _pawns)
    {
        root         = new Node;
        root->isMax  = true;
        root->pawns  = _pawns;
        root->parent = NULL;
    }
    void pushNewMove(Node *_root, std::vector<Pawn> _pawns, bool _isMax)
    {
        _root->sons.push_back(new Node);
        _root->sons.back()->pawns = _pawns;
        _root->isMax              = _isMax;
    }
    void setSonsParent(Node *_root)
    {
        for(auto it = _root->sons.begin(); it != root->sons.end(); it++)
            (*it)->parent = _root;
    }
    //    int size() { return size(mRoot); }
    //    int leaves() { return leaves(mRoot); }
    //    int height() { return height(mRoot); }
};

#endif // TREE_HPP

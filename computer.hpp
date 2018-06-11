#ifndef COMPUTER_HPP_
#define COMPUTER_HPP_

#include "Player.hpp"
#include "Tree.hpp"
#include <SFML/Graphics.hpp>

class Computer : public Player
{
    const std::vector<sf::Vector2i> validFields;
    void findMyNextMove(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn);
    void findEnemyNextMove(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn);
    bool isFieldFree(std::vector<Pawn> pawns, int x, int y);
    bool isFieldValid(int x, int y);
    int canBlackPawnMove(std::vector<Pawn> pawns, Pawn pawn);
    int canWhitePawnMove(std::vector<Pawn> pawns, Pawn pawn);
    void calculateNodesValues(Node *node);
    void setNodeValue(Node *node);
    void findNodesValue(Node *node);
    Node *findMove(Node *node);
    int findMax(Node *node);
    int findMin(Node *node);

public:
    Computer() = default;
    Computer(std::vector<sf::Vector2i> validFields) : validFields(validFields) { _isWhite = false; }
    ~Computer() {}
    void move(std::vector<Pawn> &pawns);
    void capture(std::vector<Pawn> &pawns);
};

#endif // COMPUTER_HPP

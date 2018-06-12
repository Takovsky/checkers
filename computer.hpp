#ifndef COMPUTER_HPP_
#define COMPUTER_HPP_

#include "Player.hpp"
#include "Tree.hpp"
#include <SFML/Graphics.hpp>

class Computer : public Player
{
    std::vector<std::shared_ptr<Pawn>> pawnsWhichCanCapture;
    std::vector<sf::Vector2i> validFieldsIfCanCapture;
    std::shared_ptr<Pawn> lastSelectedPawn;
    const std::vector<sf::Vector2i> validFields;
    void findMyNextMove(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn);
    void findEnemyNextMove(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn);
    bool isFieldFree(std::vector<Pawn> pawns, int x, int y);
    bool isFieldFree(std::vector<std::shared_ptr<Pawn>> pawns, sf::Vector2i pos);
    bool isFieldValid(int x, int y);
    int canBlackPawnMove(std::vector<Pawn> pawns, Pawn pawn);
    int canWhitePawnMove(std::vector<Pawn> pawns, Pawn pawn);
    void calculateNodesValues(Node *node);
    void setNodeValue(Node *node);
    void findNodesValue(Node *node);
    Node *findMove(Node *node);
    int findMax(Node *node);
    int findMin(Node *node);
    int canBlackPawnCapture(std::vector<Pawn> pawns, Pawn pawn);
    void findMyNextCapture(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn);
    bool canCapture(std::vector<std::shared_ptr<Pawn>> pawns,
                    std::vector<std::shared_ptr<Pawn>> myPawns,
                    std::vector<std::shared_ptr<Pawn>> enemyPawns);
    bool canCapture(std::vector<std::shared_ptr<Pawn>> Pawns, std::vector<std::shared_ptr<Pawn>> enemyPawns, Pawn pawn);
    int canCapture(Pawn aPawn, Pawn uPawn);
    bool canCapture(std::vector<std::shared_ptr<Pawn>> pawns, int newx, int newy, int where);
    void selectPawn(std::vector<std::shared_ptr<Pawn>> pawns, std::vector<std::shared_ptr<Pawn>> enemyPawns);
    int countCaptures(std::vector<std::shared_ptr<Pawn>> pawns,
                      std::vector<std::shared_ptr<Pawn>> enemyPawns,
                      std::shared_ptr<Pawn> pawn,
                      int offx,
                      int offy);
    bool canCapture(std::vector<std::shared_ptr<Pawn>> pawns,
                    std::vector<std::shared_ptr<Pawn>> enemyPawns,
                    int x,
                    int y);
    int canCapture(int x, int y, Pawn uPawn);
    void capture(std::vector<std::shared_ptr<Pawn>> &pawns, std::vector<std::shared_ptr<Pawn>> &enemyPawns);
    int canCapture(std::vector<std::shared_ptr<Pawn>> pawns,
                   std::vector<std::shared_ptr<Pawn>> enemyPawns,
                   Pawn pawn,
                   bool ovr);
    void removeCapturedPawn(std::vector<std::shared_ptr<Pawn> > &pawns, int offx, int offy);

public:
    Computer() = default;
    Computer(std::vector<sf::Vector2i> validFields) : validFields(validFields) { _isWhite = false; }
    ~Computer() {}
    void move(std::vector<Pawn> &pawns);
    void capture(std::vector<std::shared_ptr<Pawn>> &pawns,
                         std::vector<std::shared_ptr<Pawn>> &myPawns,
                         std::vector<std::shared_ptr<Pawn>> &enemyPawns);
};

#endif // COMPUTER_HPP

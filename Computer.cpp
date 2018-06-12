#include "Computer.hpp"
#include "Board.hpp"
#include "Tree.hpp"

void Computer::capture(std::vector<std::shared_ptr<Pawn>> &pawns,
                       std::vector<std::shared_ptr<Pawn>> &myPawns,
                       std::vector<std::shared_ptr<Pawn>> &enemyPawns)
{
    validFieldsIfCanCapture.clear();
    pawnsWhichCanCapture.clear();

    if(canCapture(pawns, myPawns, enemyPawns))
    {
        selectPawn(pawns, enemyPawns);
        capture(pawns, enemyPawns);
    }
}

void Computer::capture(std::vector<std::shared_ptr<Pawn>> &pawns, std::vector<std::shared_ptr<Pawn>> &enemyPawns)
{
    int where = 0;
    while((where = canCapture(pawns, enemyPawns, *lastSelectedPawn, true)) != 0)
    {
        if(where == 1 and isFieldValid(lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                       lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE)) // upleft
        {
            removeCapturedPawn(enemyPawns, lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE);
            removeCapturedPawn(pawns, lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE);
            lastSelectedPawn->getSprite().setPosition(lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                                      lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE);
        }
        else if(where == 2 and isFieldValid(lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                            lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE)) // upright
        {
            removeCapturedPawn(enemyPawns, lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE);
            removeCapturedPawn(pawns, lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE);
            lastSelectedPawn->getSprite().setPosition(lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                                      lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE);
        }
        else if(where == 3 and isFieldValid(lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                            lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE)) // downleft
        {
            removeCapturedPawn(enemyPawns, lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE);
            removeCapturedPawn(pawns, lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE);
            lastSelectedPawn->getSprite().setPosition(lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                                      lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE);
        }
        else if(where == 4 and isFieldValid(lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                            lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE)) // downright
        {
            removeCapturedPawn(enemyPawns, lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE);
            removeCapturedPawn(pawns, lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                               lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE);
            lastSelectedPawn->getSprite().setPosition(lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                                      lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE);
        }
    }
}

void Computer::removeCapturedPawn(std::vector<std::shared_ptr<Pawn>> &pawns, int offx, int offy)
{
    int x = lastSelectedPawn->getSprite().getPosition().x + offx;
    int y = lastSelectedPawn->getSprite().getPosition().y + offy;
    x /= 2;
    y /= 2;

    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(it->get()->getSprite().getPosition().x == x and it->get()->getSprite().getPosition().y == y)
        {
            pawns.erase(it);
        }
}

void Computer::move(std::vector<Pawn> &pawns)
{
    Tree treeOfMoves;
    treeOfMoves.setRoot(pawns);
    std::vector<Node *> tmpRoots;
    bool first = true;
    for(int i = 0; i < 2; i++)
    {
        if(i % 2)
        {
            if(first)
            {
                for(auto it = pawns.begin(); it != pawns.end(); it++)
                {
                    if(!it->isWhite())
                        findMyNextMove(treeOfMoves, treeOfMoves.root, pawns, *it);
                }
                first = false;
                treeOfMoves.setSonsParent(treeOfMoves.root);
                tmpRoots.push_back(treeOfMoves.root);
            }
            else
            {
                for(auto it = tmpRoots.begin(); it != tmpRoots.end(); it++)
                {
                    for(auto it2 = (*it)->sons.begin(); it2 != (*it)->sons.end(); it2++)
                    {
                        for(auto it3 = (*it2)->pawns.begin(); it3 != (*it2)->pawns.end(); it3++)
                        {
                            if(!it3->isWhite())
                                findMyNextMove(treeOfMoves, *it2, (*it2)->pawns, *it3);
                        }
                        treeOfMoves.setSonsParent(*it2);
                        tmpRoots.push_back(*it2);
                    }
                    tmpRoots.erase(it);
                }
            }
        }
        else
        {
            for(auto it = tmpRoots.begin(); it != tmpRoots.end(); it++)
            {
                for(auto it2 = (*it)->sons.begin(); it2 != (*it)->sons.end(); it2++)
                {
                    for(auto it3 = (*it2)->pawns.begin(); it3 != (*it2)->pawns.end(); it3++)
                    {
                        if(it3->isWhite())
                            findEnemyNextMove(treeOfMoves, *it2, (*it2)->pawns, *it3);
                    }
                    treeOfMoves.setSonsParent(*it2);
                    tmpRoots.push_back(*it2);
                }
                tmpRoots.erase(it);
            }
        }
    }

    calculateNodesValues(treeOfMoves.root);
    findNodesValue(treeOfMoves.root);
    Node *tmpNode = findMove(treeOfMoves.root);
    pawns.clear();
    for(auto it = tmpNode->pawns.begin(); it != tmpNode->pawns.end(); it++)
        pawns.push_back(*it);
}

void Computer::findMyNextCapture(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn)
{
    bool canCapture = canBlackPawnCapture(pawns, pawn);
    for(auto it = pawns.begin(); it != pawns.end(); it++)
    {
        if(pawn.getSprite().getPosition() == it->getSprite().getPosition())
        {
            if(canCapture == 1)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y - 2 * FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y + 2 * FIELD_SIZE);
            }
            else if(canCapture == 2)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y - 2 * FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y + 2 * FIELD_SIZE);
            }
            else if(canCapture == 3)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y - 2 * FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y + 2 * FIELD_SIZE);
                it->getSprite().setPosition(it->getSprite().getPosition().x + 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y - 2 * FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x - 2 * FIELD_SIZE,
                                            it->getSprite().getPosition().y + 2 * FIELD_SIZE);
            }
            break;
        }
    }
}

void Computer::findEnemyNextMove(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn)
{
    int canMove = canWhitePawnMove(pawns, pawn);
    for(auto it = pawns.begin(); it != pawns.end(); it++)
    {
        if(pawn.getSprite().getPosition() == it->getSprite().getPosition())
        {
            if(canMove == 1)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
            }
            else if(canMove == 2)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
            }
            else if(canMove == 3)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
                tree.pushNewMove(_root, pawns, true);
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
            }
            break;
        }
    }
}

void Computer::findMyNextMove(Tree &tree, Node *_root, std::vector<Pawn> pawns, Pawn pawn)
{
    int canMove = canBlackPawnMove(pawns, pawn);
    for(auto it = pawns.begin(); it != pawns.end(); it++)
    {
        if(pawn.getSprite().getPosition() == it->getSprite().getPosition())
        {
            if(canMove == 1)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
                tree.pushNewMove(_root, pawns, false);
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
            }
            else if(canMove == 2)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
                tree.pushNewMove(_root, pawns, false);
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
            }
            else if(canMove == 3)
            {
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
                tree.pushNewMove(_root, pawns, false);
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
                it->getSprite().setPosition(it->getSprite().getPosition().x + FIELD_SIZE,
                                            it->getSprite().getPosition().y + FIELD_SIZE);
                tree.pushNewMove(_root, pawns, false);
                it->getSprite().setPosition(it->getSprite().getPosition().x - FIELD_SIZE,
                                            it->getSprite().getPosition().y - FIELD_SIZE);
            }
            break;
        }
    }
}

int Computer::canBlackPawnCapture(std::vector<Pawn> pawns, Pawn pawn)
{
    int ret   = 0;
    int x     = pawn.getSprite().getPosition().x;
    int y     = pawn.getSprite().getPosition().y;
    bool left = isFieldFree(pawns, x - 2 * FIELD_SIZE, y - 2 * FIELD_SIZE) and
                isFieldValid(x - 2 * FIELD_SIZE, y - 2 * FIELD_SIZE);
    bool right = isFieldFree(pawns, x + 2 * FIELD_SIZE, y - 2 * FIELD_SIZE) and
                 isFieldValid(x + 2 * FIELD_SIZE, y - 2 * FIELD_SIZE);

    if(left and !right)
        return 1;
    if(!left and right)
        return 2;
    if(left and right)
        return 3;

    return 0;
}

int Computer::canWhitePawnMove(std::vector<Pawn> pawns, Pawn pawn)
{
    int ret   = 0;
    int x     = pawn.getSprite().getPosition().x;
    int y     = pawn.getSprite().getPosition().y;
    bool left = isFieldFree(pawns, x - FIELD_SIZE, y - FIELD_SIZE) and
                isFieldValid(x - FIELD_SIZE, y - FIELD_SIZE);
    bool right = isFieldFree(pawns, x + FIELD_SIZE, y - FIELD_SIZE) and
                 isFieldValid(x + FIELD_SIZE, y - FIELD_SIZE);

    if(left and !right)
        return 1;
    if(!left and right)
        return 2;
    if(left and right)
        return 3;

    return 0;
}

int Computer::canBlackPawnMove(std::vector<Pawn> pawns, Pawn pawn)
{
    int ret   = 0;
    int x     = pawn.getSprite().getPosition().x;
    int y     = pawn.getSprite().getPosition().y;
    bool left = isFieldFree(pawns, x - FIELD_SIZE, y + FIELD_SIZE) and
                isFieldValid(x - FIELD_SIZE, y + FIELD_SIZE);
    bool right = isFieldFree(pawns, x + FIELD_SIZE, y + FIELD_SIZE) and
                 isFieldValid(x + FIELD_SIZE, y + FIELD_SIZE);

    if(left and !right)
        return 1;
    if(!left and right)
        return 2;
    if(left and right)
        return 3;

    return 0;
}

bool Computer::isFieldFree(std::vector<Pawn> pawns, int x, int y)
{
    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(x == it->getSprite().getPosition().x and y == it->getSprite().getPosition().y)
            return false;

    return true;
}

bool Computer::isFieldFree(std::vector<std::shared_ptr<Pawn>> pawns, sf::Vector2i pos)
{
    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(pos.x == (*it)->getSprite().getPosition().x and pos.y == (*it)->getSprite().getPosition().y)
            return false;

    return true;
}

bool Computer::isFieldValid(int x, int y)
{
    for(auto it = validFields.begin(); it != validFields.end(); it++)
        if(x == it->x and y == it->y)
            return true;

    return false;
}

void Computer::calculateNodesValues(Node *node)
{
    setNodeValue(node);
    for(auto it = node->sons.begin(); it != node->sons.end(); it++)
    {
        calculateNodesValues(*it);
    }
}

void Computer::setNodeValue(Node *node)
{
    for(auto it = node->pawns.begin(); it != node->pawns.end(); it++)
    {
        if(!it->isWhite())
        {
            int x = it->getSprite().getPosition().x;
            int y = it->getSprite().getPosition().y;
            if(y < 2 * FIELD_SIZE + OUTBOARD and y > 0 * FIELD_SIZE + OUTBOARD)
                node->value += 4;
            else if(y < 4 * FIELD_SIZE + OUTBOARD and y >= 2 * FIELD_SIZE + OUTBOARD)
                node->value += 3;
            else if(y < 6 * FIELD_SIZE + OUTBOARD and y >= 4 * FIELD_SIZE + OUTBOARD)
                node->value += 2;
            else if(y < 8 * FIELD_SIZE + OUTBOARD and y >= 6 * FIELD_SIZE + OUTBOARD)
                node->value += 1;
            if((y < OUTBOARD + FIELD_SIZE or y >= BOARD_SIZE - FIELD_SIZE - OUTBOARD) and
               (x < FIELD_SIZE + OUTBOARD or x >= BOARD_SIZE - OUTBOARD - FIELD_SIZE))
                node->value += 3;
            else if(((y < OUTBOARD + 2 * FIELD_SIZE and y >= OUTBOARD + 1 * FIELD_SIZE) or
                     (y >= BOARD_SIZE - 2 * FIELD_SIZE - OUTBOARD and y < BOARD_SIZE - FIELD_SIZE - OUTBOARD)) and
                    ((x < 2 * FIELD_SIZE + OUTBOARD and x >= FIELD_SIZE + OUTBOARD) or
                     (x >= BOARD_SIZE - OUTBOARD - 2 * FIELD_SIZE and x < BOARD_SIZE - FIELD_SIZE - OUTBOARD)))
                node->value += 2;
            else
                node->value += 1;
        }
        else if(it->isWhite())
        {
            int x = it->getSprite().getPosition().x;
            int y = it->getSprite().getPosition().y;
            if(y < 2 * FIELD_SIZE + OUTBOARD and y > 0 * FIELD_SIZE + OUTBOARD)
                node->value -= 4;
            else if(y < 4 * FIELD_SIZE + OUTBOARD and y >= 2 * FIELD_SIZE + OUTBOARD)
                node->value -= 3;
            else if(y < 6 * FIELD_SIZE + OUTBOARD and y >= 4 * FIELD_SIZE + OUTBOARD)
                node->value -= 2;
            else if(y < 8 * FIELD_SIZE + OUTBOARD and y >= 6 * FIELD_SIZE + OUTBOARD)
                node->value -= 1;
            if((y < OUTBOARD + FIELD_SIZE or y >= BOARD_SIZE - FIELD_SIZE - OUTBOARD) and
               (x < FIELD_SIZE + OUTBOARD or x >= BOARD_SIZE - OUTBOARD - FIELD_SIZE))
                node->value -= 3;
            else if(((y < OUTBOARD + 2 * FIELD_SIZE and y >= OUTBOARD + 1 * FIELD_SIZE) or
                     (y >= BOARD_SIZE - 2 * FIELD_SIZE - OUTBOARD and y < BOARD_SIZE - FIELD_SIZE - OUTBOARD)) and
                    ((x < 2 * FIELD_SIZE + OUTBOARD and x >= FIELD_SIZE + OUTBOARD) or
                     (x >= BOARD_SIZE - OUTBOARD - 2 * FIELD_SIZE and x < BOARD_SIZE - FIELD_SIZE - OUTBOARD)))
                node->value -= 2;
            else
                node->value -= 1;
        }
    }
}

Node *Computer::findMove(Node *node)
{
    int max   = (*node->sons.begin())->value;
    int i     = 1;
    int index = 0;
    if(node->sons.size() > 1)
    {
        for(auto it = node->sons.begin() + 1; it != node->sons.end(); it++, i++)
        {
            if((*it)->value > max)
            {
                max   = (*it)->value;
                index = i;
            }
        }
    }
    return node->sons[index];
}

void Computer::findNodesValue(Node *node)
{
    for(auto it = node->sons.begin(); it != node->sons.end(); it++)
    {
        findNodesValue(*it);
        if(node->isMax)
            node->value = findMax(node);
        else
            node->value = findMin(node);
    }
}

int Computer::findMax(Node *node)
{
    int max = node->sons[0]->value;
    if(node->sons.size() > 1)
        for(auto it = node->sons.begin() + 1; it != node->sons.end(); it++)
        {
            if(max < (*it)->value)
                max = (*it)->value;
        }

    return max;
}

int Computer::findMin(Node *node)
{
    int min = node->sons[0]->value;
    if(node->sons.size() > 1)
        for(auto it = node->sons.begin() + 1; it != node->sons.end(); it++)
        {
            if(min > (*it)->value)
                min = (*it)->value;
        }

    return min;
}

bool Computer::canCapture(std::vector<std::shared_ptr<Pawn>> pawns, int newx, int newy, int where)
{
    sf::Vector2i field;
    bool ret = true;
    if(where == 1) // upleft
    {
        field.x = newx - FIELD_SIZE;
        field.y = newy - FIELD_SIZE;
        ret     = isFieldFree(pawns, field);
    }
    else if(where == 2) // upright
    {
        field.x = newx + FIELD_SIZE;
        field.y = newy - FIELD_SIZE;
        ret     = isFieldFree(pawns, field);
    }
    else if(where == 3) // downleft
    {
        field.x = newx - FIELD_SIZE;
        field.y = newy + FIELD_SIZE;
        ret     = isFieldFree(pawns, field);
    }
    else if(where == 4) // downright
    {
        field.x = newx + FIELD_SIZE;
        field.y = newy + FIELD_SIZE;
        ret     = isFieldFree(pawns, field);
    }

    if(!isFieldValid(field.x, field.y))
        return false;
    validFieldsIfCanCapture.push_back(field);
    return ret;
}

int Computer::canCapture(Pawn aPawn, Pawn uPawn)
{
    if(aPawn.getSprite().getPosition().x - FIELD_SIZE == uPawn.getSprite().getPosition().x and
       aPawn.getSprite().getPosition().y - FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 1; // upleft
    if(aPawn.getSprite().getPosition().x + FIELD_SIZE == uPawn.getSprite().getPosition().x and
       aPawn.getSprite().getPosition().y - FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 2; // upright
    if(aPawn.getSprite().getPosition().x - FIELD_SIZE == uPawn.getSprite().getPosition().x and
       aPawn.getSprite().getPosition().y + FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 3; // downleft
    if(aPawn.getSprite().getPosition().x + FIELD_SIZE == uPawn.getSprite().getPosition().x and
       aPawn.getSprite().getPosition().y + FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 4; // downright

    return 0;
}

int Computer::canCapture(int x, int y, Pawn uPawn)
{
    if(x - FIELD_SIZE == uPawn.getSprite().getPosition().x and
       y - FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 1; // upleft
    if(x + FIELD_SIZE == uPawn.getSprite().getPosition().x and
       y - FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 2; // upright
    if(x - FIELD_SIZE == uPawn.getSprite().getPosition().x and
       y + FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 3; // downleft
    if(x + FIELD_SIZE == uPawn.getSprite().getPosition().x and
       y + FIELD_SIZE == uPawn.getSprite().getPosition().y)
        return 4; // downright

    return 0;
}

bool Computer::canCapture(std::vector<std::shared_ptr<Pawn>> pawns,
                          std::vector<std::shared_ptr<Pawn>> enemyPawns,
                          Pawn pawn)
{
    int where = 0;
    bool ret  = false;
    for(auto it = enemyPawns.begin(); it != enemyPawns.end(); it++)
        if((where = canCapture(pawn, *it->get())) and
           canCapture(pawns, it->get()->getSprite().getPosition().x,
                      it->get()->getSprite().getPosition().y, where))
            ret = true;

    return ret;
}

int Computer::canCapture(std::vector<std::shared_ptr<Pawn>> pawns,
                         std::vector<std::shared_ptr<Pawn>> enemyPawns,
                         Pawn pawn,
                         bool ovr)
{
    int where = 0;
    for(auto it = enemyPawns.begin(); it != enemyPawns.end(); it++)
    {
        if((where = canCapture(pawn, *it->get())) and
           canCapture(pawns, it->get()->getSprite().getPosition().x,
                      it->get()->getSprite().getPosition().y, where))
            return where;
    }

    return where;
}

bool Computer::canCapture(std::vector<std::shared_ptr<Pawn>> pawns,
                          std::vector<std::shared_ptr<Pawn>> enemyPawns,
                          int x,
                          int y)
{
    int where = 0;
    bool ret  = false;
    for(auto it = enemyPawns.begin(); it != enemyPawns.end(); it++)
        if((where = canCapture(x, y, *it->get())) and canCapture(pawns, x, y, where))
            ret = true;

    return ret;
}

bool Computer::canCapture(std::vector<std::shared_ptr<Pawn>> pawns,
                          std::vector<std::shared_ptr<Pawn>> myPawns,
                          std::vector<std::shared_ptr<Pawn>> enemyPawns)
{
    validFieldsIfCanCapture.clear();
    bool ret = false;
    for(auto it = myPawns.begin(); it != myPawns.end(); it++)
        if(canCapture(pawns, enemyPawns, *it->get()))
        {
            pawnsWhichCanCapture.push_back(*it);
            ret = true;
        }

    return ret;
}

void Computer::selectPawn(std::vector<std::shared_ptr<Pawn>> pawns, std::vector<std::shared_ptr<Pawn>> enemyPawns)
{
    int max          = countCaptures(pawns, enemyPawns, pawnsWhichCanCapture[0], 0, 0);
    lastSelectedPawn = pawnsWhichCanCapture[0];
    if(pawnsWhichCanCapture.size() > 1)
    {
        for(auto it = pawnsWhichCanCapture.begin(); it != pawnsWhichCanCapture.end(); it++)
        {
            int tmp = countCaptures(pawns, enemyPawns, *it, 0, 0);
            if(tmp > max)
            {
                max              = tmp;
                lastSelectedPawn = *it;
            }
        }
    }
}

int Computer::countCaptures(std::vector<std::shared_ptr<Pawn>> pawns,
                            std::vector<std::shared_ptr<Pawn>> enemyPawns,
                            std::shared_ptr<Pawn> pawn,
                            int offx,
                            int offy)
{
    int x = pawn->getSprite().getPosition().x + offx;
    int y = pawn->getSprite().getPosition().y + offy;
    for(int i = 0; i < validFieldsIfCanCapture.size(); i++)
    {
        if(validFieldsIfCanCapture[i].x == x - 2 * FIELD_SIZE and validFieldsIfCanCapture[i].y == y - 2 * FIELD_SIZE)
        {
            validFieldsIfCanCapture.clear();
            canCapture(pawns, enemyPawns, x, y);
            return countCaptures(pawns, enemyPawns, pawn, FIELD_SIZE * -2, FIELD_SIZE * -2) + 1;
        }
        else if(validFieldsIfCanCapture[i].x == x + 2 * FIELD_SIZE and
                validFieldsIfCanCapture[i].y == y - 2 * FIELD_SIZE)
        {
            validFieldsIfCanCapture.clear();
            canCapture(pawns, enemyPawns, x, y);
            return countCaptures(pawns, enemyPawns, pawn, FIELD_SIZE * 2, FIELD_SIZE * -2) + 1;
        }
        else if(validFieldsIfCanCapture[i].x == x - 2 * FIELD_SIZE and
                validFieldsIfCanCapture[i].y == y + 2 * FIELD_SIZE)
        {
            validFieldsIfCanCapture.clear();
            canCapture(pawns, enemyPawns, x, y);
            return countCaptures(pawns, enemyPawns, pawn, FIELD_SIZE * -2, FIELD_SIZE * 2) + 1;
        }
        else if(validFieldsIfCanCapture[i].x == x + 2 * FIELD_SIZE and
                validFieldsIfCanCapture[i].y == y + 2 * FIELD_SIZE)
        {
            validFieldsIfCanCapture.clear();
            canCapture(pawns, enemyPawns, x, y);
            return countCaptures(pawns, enemyPawns, pawn, FIELD_SIZE * 2, FIELD_SIZE * 2) + 1;
        }
        else
        {
            validFieldsIfCanCapture.clear();
            for(auto it = pawnsWhichCanCapture.begin(); it != pawnsWhichCanCapture.end(); it++)
                canCapture(pawns, enemyPawns, *it->get());
            return 0;
        }
    }
}

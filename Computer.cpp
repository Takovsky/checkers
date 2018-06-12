#include "Computer.hpp"
#include "Board.hpp"
#include "Tree.hpp"

sf::Vector2i Computer::capture(std::vector<Pawn> &pawns,
                               std::vector<Pawn> &pawnsWhichCanCapture,
                               std::vector<sf::Vector2i> &validFieldsIfCanCapture)
{
    int where = 0;
    int x, y;
    int index;
    int i = 0;
    for(auto it = pawns.begin(); it != pawns.end(); it++, i++)
    {
        if(pawnsWhichCanCapture[0].getSprite().getPosition() == it->getSprite().getPosition())
            index = i;
    }
    if((where = canCapture(pawnsWhichCanCapture[0], validFieldsIfCanCapture) != 0))
    {
        if(where == 1)
        {
            x = pawnsWhichCanCapture[0].getSprite().getPosition().x - 2 * FIELD_SIZE;
            y = pawnsWhichCanCapture[0].getSprite().getPosition().y - 2 * FIELD_SIZE;
        }
        else if(where == 2)
        {
            x = pawnsWhichCanCapture[0].getSprite().getPosition().x + 2 * FIELD_SIZE;
            y = pawnsWhichCanCapture[0].getSprite().getPosition().y - 2 * FIELD_SIZE;
        }
        else if(where == 3)
        {
            x = pawnsWhichCanCapture[0].getSprite().getPosition().x - 2 * FIELD_SIZE;
            y = pawnsWhichCanCapture[0].getSprite().getPosition().y + 2 * FIELD_SIZE;
        }
        else if(where == 4)
        {
            x = pawnsWhichCanCapture[0].getSprite().getPosition().x + 2 * FIELD_SIZE;
            y = pawnsWhichCanCapture[0].getSprite().getPosition().y + 2 * FIELD_SIZE;
        }

        pawnsWhichCanCapture[0].getSprite().setPosition(x, y);
    }

    sf::Vector2i capturedPawn;
    capturedPawn.x =
    (pawnsWhichCanCapture[0].getSprite().getPosition().x + pawns[index].getSprite().getPosition().x) / 2;
    capturedPawn.y =
    (pawnsWhichCanCapture[0].getSprite().getPosition().y + pawns[index].getSprite().getPosition().y) / 2;

    pawns[index].getSprite().setPosition(pawnsWhichCanCapture[0].getSprite().getPosition().x,
                                         pawnsWhichCanCapture[0].getSprite().getPosition().y);

    return capturedPawn;
}

int Computer::canCapture(Pawn pawn, std::vector<sf::Vector2i> validFieldsIfCanCapture)
{
    int newx = pawn.getSprite().getPosition().x;
    int newy = pawn.getSprite().getPosition().y;
    for(auto it = validFieldsIfCanCapture.begin(); it != validFieldsIfCanCapture.end(); it++)
    {
        if(it->x == newx - 2 * FIELD_SIZE and it->y == newy - 2 * FIELD_SIZE)
            return 1;
        if(it->x == newx + 2 * FIELD_SIZE and it->y == newy - 2 * FIELD_SIZE)
            return 2;
        if(it->x == newx - 2 * FIELD_SIZE and it->y == newy + 2 * FIELD_SIZE)
            return 3;
        if(it->x == newx + 2 * FIELD_SIZE and it->y == newy + 2 * FIELD_SIZE)
            return 4;
    }

    return 0;
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

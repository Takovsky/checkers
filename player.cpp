#include "Player.hpp"
#include "Board.hpp"
#include <SFML/Graphics.hpp>
#include <string>

std::shared_ptr<Pawn> Player::choosenPawn()
{
    return pawns[lastChoosenPawn];
}

void Player::popPawn(int x, int y)
{
    for(auto it = pawns.begin(); it != pawns.end(); it++)
    {
        if(it->get()->getSprite().getPosition().x == x and it->get()->getSprite().getPosition().y == y)
        {
            pawns.erase(it);
            return;
        }
    }
}

bool Player::isMyPawn(std::shared_ptr<Pawn> pawn)
{
    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(*it == pawn)
            return true;

    return false;
}

bool Player::isPawnClicked(int x, int y)
{
    int i = 0;
    for(auto it = pawns.begin(); it != pawns.end(); it++, i++)
    {
        if(isPawnFilledInCoords(*it->get(), x, y))
        {
            lastChoosenPawn = i;
            return true;
        }
    }

    return false;
}

bool Player::isFieldClicked(int x, int y)
{
    return x > OUTBOARD and x < BOARD_SIZE - OUTBOARD and y > OUTBOARD and y < BOARD_SIZE - OUTBOARD;
}

bool Player::isPawnFilledInCoords(Pawn pawn, int x, int y)
{
    if(pawn.getSprite().getPosition().x < x and pawn.getSprite().getPosition().x + FIELD_SIZE > x and
       pawn.getSprite().getPosition().y < y and pawn.getSprite().getPosition().y + FIELD_SIZE > y)
        return true;

    return false;
}

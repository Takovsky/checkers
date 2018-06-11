#include "Pawn.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

Pawn::Pawn(bool isWhite, int x, int y) : _isWhite(isWhite), _isKing(false)
{
    startPos.x = x;
    startPos.y = y;

    loadUnselectedImage();

    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setPosition(224, 224);
}

Pawn::Pawn(const Pawn &pawn)
{
    _isWhite = pawn._isWhite;
    startPos = pawn.startPos;
    _isKing  = pawn._isKing;
    sprite   = pawn.sprite;
    texture  = pawn.texture;
}

void Pawn::setAtStartPosition()
{
    sprite.setPosition(startPos);
}

void Pawn::loadUnselectedImage()
{
    if(_isWhite and !_isKing)
        texture.loadFromFile("images/pawns.png", sf::IntRect(0, 0, 56, 56));
    else if(!_isWhite and !_isKing)
        texture.loadFromFile("images/pawns.png", sf::IntRect(0, 57, 56, 56));
    else if(_isWhite and _isKing)
        texture.loadFromFile("images/pawns.png", sf::IntRect(57, 0, 56, 56));
    else
        texture.loadFromFile("images/pawns.png", sf::IntRect(57, 57, 56, 56));
}

void Pawn::loadSelectedImage()
{
    if(_isWhite and !_isKing)
        texture.loadFromFile("images/pawnschoosen.png", sf::IntRect(0, 0, 56, 56));
    else if(!_isWhite and !_isKing)
        texture.loadFromFile("images/pawnschoosen.png", sf::IntRect(0, 57, 56, 56));
    else if(_isWhite and _isKing)
        texture.loadFromFile("images/pawnschoosen.png", sf::IntRect(57, 0, 56, 56));
    else
        texture.loadFromFile("images/pawnschoosen.png", sf::IntRect(57, 57, 56, 56));
}

sf::Vector2f Pawn::normalizedVectorFromMidToStartPos()
{
    sf::Vector2f direction = startPos - sprite.getPosition();
    float length           = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
    direction.x            = direction.x / length;
    direction.y            = direction.y / length;

    return direction;
}

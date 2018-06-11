#ifndef PAWN_HPP_
#define PAWN_HPP_

#include <SFML/Graphics.hpp>

class Pawn
{
    bool _isWhite;
    sf::Vector2f startPos;
    bool _isKing;
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Pawn() = default;
    Pawn(bool _isWhite, int x, int y);
    Pawn(const Pawn &pawn);
    bool isWhite() const { return _isWhite; }
    sf::Sprite &getSprite() { return sprite; }
    sf::Sprite getSprite() const { return sprite; }
    float startX() const { return startPos.x; }
    float &startX() { return startPos.x; }
    float startY() const { return startPos.y; }
    float &staryY() { return startPos.y; }
    bool isKing() { return _isKing; }
    void updateToKing();
    void loadSelectedImage();
    void loadUnselectedImage();
    sf::Vector2f normalizedVectorFromMidToStartPos();
    void setAtStartPosition();
};

#endif

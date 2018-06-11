#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Pawn.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player
{
    const std::vector<sf::Vector2i> validFields;
    sf::Texture texture;
    sf::Sprite selectedField;
    int lastChoosenPawn;

    bool isMoveCorrect();
    void handleMouseClick(int x, int y);
    Pawn pawnClicked(int x, int y);
    bool isPawnFilledInCoords(Pawn pawn, int x, int y);
    bool isFieldFilledInCoords(sf::Vector2i field, int x, int y);
    void loadFieldSelectedImage(sf::Vector2i field);
    bool fieldClicked(int x, int y);

protected:
    bool _isWhite;

public:
    std::vector<std::shared_ptr<Pawn>> pawns;
    Player(std::vector<sf::Vector2i> validFields, bool isWhite)
    : validFields(validFields), _isWhite(isWhite)
    {
    }
    Player() = default;
    virtual ~Player() {}
    virtual void move(std::vector<Pawn> &pawns) {}
    virtual void capture(std::vector<Pawn> &pawns) {}
    void pushPawn(std::shared_ptr<Pawn> pawn) { pawns.push_back(pawn); }
    std::shared_ptr<Pawn> choosenPawn();
    bool isPawnClicked(int x, int y);
    bool isFieldClicked(int x, int y);
    bool isMyPawn(std::shared_ptr<Pawn> pawn);
    bool isWhite() const { return _isWhite; }
    void popPawn(int x, int y);
};

#endif

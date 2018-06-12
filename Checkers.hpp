#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_

#include "Computer.hpp"
#include "GameMode.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <vector>

class Checkers
{
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::shared_ptr<Pawn>> pawns;
    std::vector<std::shared_ptr<Pawn>> pawnsWhichCanCapture;
    std::vector<sf::Vector2i> validFields;
    std::vector<sf::Vector2i> validFieldsIfCanCapture;
    std::vector<sf::Sprite> selectedSprites;
    std::shared_ptr<Pawn> lastSelectedPawn;
    sf::Texture boardTexture;
    sf::Texture selectedTexture;
    sf::Texture unselectedTexture;
    sf::Sprite boardSprite;
    sf::RenderWindow &window;
    GameMode gameMode;
    int lastSelectedField;
    int activePlayer;
    int unactivePlayer;
    int whitePoints;
    int blackPoints;

    void play();
    void setBoardSprite();
    void setBoardToPlay();
    void setPlayers();
    void setPawns();
    void showPawns();
    void movePawnToStartPositions(std::shared_ptr<Pawn> pawn, sf::Vector2f direction, sf::Event event);
    void closeWindowIfCloseClicked(sf::Event event);
    void setValidFields();
    bool isPawnOnField(Pawn pawn, sf::Vector2i field);
    bool isPawnOnField(sf::Vector2i field);
    void setSelectedAndUnselectedFieldTexture();
    void selectField(Pawn pawn);
    void selectField(sf::Vector2i field);
    void movePawnToStartPositions(std::shared_ptr<Pawn> pawn);
    bool isValidFieldClicked(int x, int y);
    bool isFieldValid(int x, int y);
    bool canMove(int x, int y);
    void setSelectedFieldsAsUnselectedAfterMove();
    bool canCapture();
    bool canCapture(Pawn pawn);
    bool canCapture(int newx, int newy, int where);
    int canCapture(Pawn aPawn, Pawn uPawn);
    void Capture(sf::Event event, bool &capturing, bool &isNewMove);
    void Capture(sf::Event event, bool &finishedCapturing);
    void changeActivePlayer();
    bool isCapturingPawnClicked(int x, int y);
    bool isPawnFilledInCoords(Pawn pawn, int x, int y);
    bool isValidFieldWhileCapturingClicked(int x, int y);
    void findLastSelectedField(int x, int y);
    void removeCapturedPawn();
    bool checkIfAnyPawnAtEndOfBoard();
    void addPointAndRemovePawn(int x, int y, bool isWhite);
    void removeCapturedPawn(sf::Vector2i vec);
    void removeCapturedPawn(int index);

public:
    Checkers(sf::RenderWindow &window) : window(window), whitePoints(0), blackPoints(0) {}
    void run(GameMode mode);
};

#endif

#include "Checkers.hpp"
#include "Board.hpp"
#include "GameMode.hpp"
#include <SFML/Graphics.hpp>
#include <cstring>
#include <iostream>
#include <typeinfo>

void Checkers::run(GameMode mode)
{
    gameMode       = mode;
    activePlayer   = 0;
    unactivePlayer = 1;

    setSelectedAndUnselectedFieldTexture();
    setValidFields();
    setBoardSprite();
    setPlayers();
    setPawns();
    setBoardToPlay();
    play();
}

void Checkers::setSelectedAndUnselectedFieldTexture()
{
    selectedTexture.loadFromFile("images/selectedfield.png");
    unselectedTexture.loadFromFile("images/unselectedfield.png");
    for(int i = 0; i < 4; i++)
    {
        selectedSprites.push_back(sf::Sprite(selectedTexture));
        selectedSprites.back().setPosition(-FIELD_SIZE, 0);
    }
}

void Checkers::setValidFields()
{
    sf::Vector2i validField;
    int xDistance = 56;
    int yDistance = 0;

    for(int i = 0; i < 36;)
    {
        validField.x = OUTBOARD + xDistance;
        validField.y = OUTBOARD + yDistance;
        xDistance += 112;

        validFields.push_back(validField);

        i++;

        if(i % 8 == 0)
        {
            xDistance = 56;
            yDistance += 56;
        }
        else if(i % 4 == 0)
        {
            xDistance = 0;
            yDistance += 56;
        }
    }
}

void Checkers::setBoardSprite()
{
    boardTexture.loadFromFile("images/board.png");
    boardSprite.setTexture(boardTexture);
}

void Checkers::play()
{
    sf::Font font;
    font.loadFromFile("times.ttf");
    sf::Text text;
    text.setFont(font);
    text.setPosition(180, 0);
    text.setColor(sf::Color::White);
    text.setCharacterSize(18);
    std::string score;
    bool capturing = false;

    bool isNewMove = false;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
            if(event.type == sf::Event::Closed)
                window.close();

        if(isNewMove)
        {
            isNewMove = false;
            if(canCapture())
                capturing = true;
        }

        if(!strcmp(typeid(*players[activePlayer]).name(), "8Computer"))
        {
            std::vector<Pawn> pawnsCopy;
            std::vector<Pawn> capturingPawnsCopy;
            sf::Vector2i capturedPawn;
            for(auto it = pawns.begin(); it != pawns.end(); it++)
                pawnsCopy.push_back(*it->get());
            for(auto it = pawnsWhichCanCapture.begin(); it != pawnsWhichCanCapture.end(); it++)
                capturingPawnsCopy.push_back(*it->get());
            if(capturing)
                capturedPawn =
                players[activePlayer]->capture(pawnsCopy, capturingPawnsCopy, validFieldsIfCanCapture);
            else
                players[activePlayer]->move(pawnsCopy);
            for(int i = 0; i < pawns.size(); i++)
            {
                if(pawns[i]->getSprite().getPosition() != pawnsCopy[i].getSprite().getPosition())
                {
                    findLastSelectedField(pawns[i]->getSprite().getPosition().x,
                                          pawns[i]->getSprite().getPosition().y);
                    pawns[i]->getSprite().setPosition(pawnsCopy[i].getSprite().getPosition());
                    lastSelectedPawn = pawns[i];
                    selectField(validFields[lastSelectedField]);
                    selectField(*lastSelectedPawn);
                }
            }
            if(capturing)
                removeCapturedPawn(capturedPawn);
            changeActivePlayer();
            isNewMove = true;
            capturing = false;
        }
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if(!capturing and
               players[activePlayer]->isPawnClicked(event.mouseButton.x, event.mouseButton.y))
            {
                lastSelectedPawn = players[activePlayer]->choosenPawn();
                selectField(*lastSelectedPawn);
            }

            if(!capturing and players[activePlayer]->isMyPawn(lastSelectedPawn) and
               canMove(event.mouseButton.x, event.mouseButton.y))
            {
                lastSelectedPawn->getSprite().setPosition(
                static_cast<sf::Vector2f>(validFields[lastSelectedField]));
                selectField(validFields[lastSelectedField]);
                setSelectedFieldsAsUnselectedAfterMove();
                changeActivePlayer();
                isNewMove = true;
            }
            if(capturing)
                Capture(event, capturing, isNewMove);
        }

        checkIfAnyPawnAtEndOfBoard();
        score = "WHITE " + std::to_string(whitePoints);
        score += ":" + std::to_string(blackPoints) + " BLACK";
        text.setString(score);
        window.clear();
        window.draw(boardSprite);
        for(auto it = pawns.begin(); it != pawns.end(); it++)
            window.draw(it->get()->getSprite());
        for(auto it = selectedSprites.begin(); it != selectedSprites.end(); it++)
            window.draw(*it);
        window.draw(text);
        window.display();
    }
}

void Checkers::checkIfAnyPawnAtEndOfBoard()
{
    for(auto it = pawns.begin(); it != pawns.end(); it++)
    {
        if(it->get()->isWhite() and it->get()->getSprite().getPosition().y == OUTBOARD)
        {
            addPointAndRemovePawn(it->get()->getSprite().getPosition().x,
                                  it->get()->getSprite().getPosition().y, true);
            return;
        }
        else if(!it->get()->isWhite() and it->get()->getSprite().getPosition().y == BOARD_SIZE - OUTBOARD - FIELD_SIZE)
        {
            addPointAndRemovePawn(it->get()->getSprite().getPosition().x,
                                  it->get()->getSprite().getPosition().y, false);
            return;
        }
    }
}

void Checkers::addPointAndRemovePawn(int x, int y, bool isWhite)
{
    if(isWhite)
    {
        whitePoints += 1;
        if(players[activePlayer]->isWhite())
            players[activePlayer]->popPawn(x, y);
        else
            players[unactivePlayer]->popPawn(x, y);
    }
    else
    {
        blackPoints += 1;
        if(!players[activePlayer]->isWhite())
            players[activePlayer]->popPawn(x, y);
        else
            players[unactivePlayer]->popPawn(x, y);
    }

    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(it->get()->getSprite().getPosition().x == x and it->get()->getSprite().getPosition().y == y)
        {
            pawns.erase(it);
            return;
        }
}

void Checkers::removeCapturedPawn(sf::Vector2i vec)
{
    players[unactivePlayer]->popPawn(vec.x, vec.y);
    for(auto it = pawns.begin(); it != pawns.end(); it++)
    {
        if(it->get()->getSprite().getPosition().x == vec.x and
           it->get()->getSprite().getPosition().y == vec.y)
        {
            pawns.erase(it);
            return;
        }
    }
}

void Checkers::removeCapturedPawn()
{
    int x = lastSelectedPawn->getSprite().getPosition().x + validFields[lastSelectedField].x;
    int y = lastSelectedPawn->getSprite().getPosition().y + validFields[lastSelectedField].y;
    x /= 2;
    y /= 2;

    players[unactivePlayer]->popPawn(x, y);
    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(it->get()->getSprite().getPosition().x == x and it->get()->getSprite().getPosition().y == y)
        {
            pawns.erase(it);
            return;
        }
}

void Checkers::Capture(sf::Event event, bool &capturing, bool &isNewMove)
{
    bool finishedCapturing      = false;
    bool isPawnSelectedAndMoved = false;

    if(!isPawnSelectedAndMoved and isCapturingPawnClicked(event.mouseButton.x, event.mouseButton.y))
        selectField(*lastSelectedPawn);
    if(!isPawnSelectedAndMoved and
       isValidFieldWhileCapturingClicked(event.mouseButton.x, event.mouseButton.y))
    {
        isPawnSelectedAndMoved = true;
        removeCapturedPawn();
        lastSelectedPawn->getSprite().setPosition(static_cast<sf::Vector2f>(validFields[lastSelectedField]));
        selectField(validFields[lastSelectedField]);
        setSelectedFieldsAsUnselectedAfterMove();
    }
    if(isPawnSelectedAndMoved and !finishedCapturing)
    {
        validFieldsIfCanCapture.clear();
        pawnsWhichCanCapture.clear();
        pawnsWhichCanCapture.push_back(lastSelectedPawn);
        Capture(event, finishedCapturing);
    }
    if(finishedCapturing)
    {
        validFieldsIfCanCapture.clear();
        pawnsWhichCanCapture.clear();
        capturing = false;
        isNewMove = true;
        changeActivePlayer();
    }
}

void Checkers::Capture(sf::Event event, bool &finishedCapturing)
{
    if(canCapture(*lastSelectedPawn))
    {
        if(isValidFieldWhileCapturingClicked(event.mouseButton.x, event.mouseButton.y))
        {
            removeCapturedPawn();
            lastSelectedPawn->getSprite().setPosition(static_cast<sf::Vector2f>(validFields[lastSelectedField]));
            selectField(validFields[lastSelectedField]);
            setSelectedFieldsAsUnselectedAfterMove();
        }
    }
    else
        finishedCapturing = true;
}

bool Checkers::isValidFieldWhileCapturingClicked(int x, int y)
{
    for(auto it = validFieldsIfCanCapture.begin(); it != validFieldsIfCanCapture.end(); it++)
    {
        if(it->y < y and it->y + FIELD_SIZE > y and it->x < x and it->x + FIELD_SIZE > x and
           (it->y == lastSelectedPawn->getSprite().getPosition().y - 2 * FIELD_SIZE or
            it->y == lastSelectedPawn->getSprite().getPosition().y + 2 * FIELD_SIZE) and
           (it->x == lastSelectedPawn->getSprite().getPosition().x - 2 * FIELD_SIZE or
            it->x == lastSelectedPawn->getSprite().getPosition().x + 2 * FIELD_SIZE))
        {
            findLastSelectedField(it->x, it->y);
            return true;
        }
    }
    return false;
}

void Checkers::findLastSelectedField(int x, int y)
{
    int i = 0;
    for(auto it = validFields.begin(); it != validFields.end(); it++, i++)
    {
        if(it->x == x and it->y == y)
            lastSelectedField = i;
    }
}

bool Checkers::isCapturingPawnClicked(int x, int y)
{
    for(auto it = pawnsWhichCanCapture.begin(); it != pawnsWhichCanCapture.end(); it++)
    {
        if(isPawnFilledInCoords(*it->get(), x, y))
        {
            lastSelectedPawn = *it;
            return true;
        }
    }

    return false;
}

bool Checkers::isPawnFilledInCoords(Pawn pawn, int x, int y)
{
    if(pawn.getSprite().getPosition().x < x and pawn.getSprite().getPosition().x + FIELD_SIZE > x and
       pawn.getSprite().getPosition().y < y and pawn.getSprite().getPosition().y + FIELD_SIZE > y)
        return true;

    return false;
}

void Checkers::changeActivePlayer()
{
    if(activePlayer == 0)
    {
        activePlayer   = 1;
        unactivePlayer = 0;
    }
    else
    {
        activePlayer   = 0;
        unactivePlayer = 1;
    }
}

bool Checkers::canCapture(int newx, int newy, int where)
{
    sf::Vector2i field;
    bool ret = true;
    if(where == 1) // upleft
    {
        field.x = newx - FIELD_SIZE;
        field.y = newy - FIELD_SIZE;
        ret     = !isPawnOnField(field);
    }
    else if(where == 2) // upright
    {
        field.x = newx + FIELD_SIZE;
        field.y = newy - FIELD_SIZE;
        ret     = !isPawnOnField(field);
    }
    else if(where == 3) // downleft
    {
        field.x = newx - FIELD_SIZE;
        field.y = newy + FIELD_SIZE;
        ret     = !isPawnOnField(field);
    }
    else if(where == 4) // downright
    {
        field.x = newx + FIELD_SIZE;
        field.y = newy + FIELD_SIZE;
        ret     = !isPawnOnField(field);
    }

    if(!isFieldValid(field.x, field.y))
        return false;
    validFieldsIfCanCapture.push_back(field);
    return ret;
}

int Checkers::canCapture(Pawn aPawn, Pawn uPawn)
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

bool Checkers::canCapture(Pawn pawn)
{
    int where = 0;
    bool ret  = false;
    for(auto it = players[unactivePlayer]->pawns.begin(); it != players[unactivePlayer]->pawns.end(); it++)
        if((where = canCapture(pawn, *it->get())) and
           canCapture(it->get()->getSprite().getPosition().x, it->get()->getSprite().getPosition().y, where))
            ret = true;

    return ret;
}

bool Checkers::canCapture()
{
    validFieldsIfCanCapture.clear();
    bool ret = false;
    for(auto it = players[activePlayer]->pawns.begin(); it != players[activePlayer]->pawns.end(); it++)
        if(canCapture(*it->get()))
        {
            pawnsWhichCanCapture.push_back(*it);
            ret = true;
        }

    return ret;
}

void Checkers::selectField(Pawn pawn)
{
    for(auto it = validFields.begin(); it != validFields.end(); it++)
    {
        if(isPawnOnField(pawn, *it) and players[activePlayer]->isWhite())
        {
            selectedSprites[0].setTexture(selectedTexture);
            selectedSprites[0].setPosition(it->x, it->y);
        }
        else if(isPawnOnField(pawn, *it))
        {
            selectedSprites[2].setTexture(selectedTexture);
            selectedSprites[2].setPosition(it->x, it->y);
        }
    }
}

void Checkers::selectField(sf::Vector2i field)
{
    if(players[activePlayer]->isWhite())
    {
        selectedSprites[1].setTexture(selectedTexture);
        selectedSprites[1].setPosition(field.x, field.y);
    }
    else
    {
        selectedSprites[3].setTexture(selectedTexture);
        selectedSprites[3].setPosition(field.x, field.y);
    }
}

void Checkers::setSelectedFieldsAsUnselectedAfterMove()
{
    if(players[activePlayer]->isWhite())
    {
        selectedSprites[2].setTexture(unselectedTexture);
        selectedSprites[3].setTexture(unselectedTexture);
    }
    else
    {
        selectedSprites[0].setTexture(unselectedTexture);
        selectedSprites[1].setTexture(unselectedTexture);
    }
}

bool Checkers::canMove(int x, int y)
{
    if(!isValidFieldClicked(x, y))
        return false;

    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(isPawnOnField(*it->get(), validFields[lastSelectedField]))
            return false;

    return true;
}

bool Checkers::isFieldValid(int x, int y)
{

    return x >= OUTBOARD and x <= BOARD_SIZE - OUTBOARD - FIELD_SIZE and y >= OUTBOARD and
           y <= BOARD_SIZE - OUTBOARD - FIELD_SIZE;
    //    for(auto it = validFields.begin(); it != validFields.end(); it++)
    //        if(x == it->x and y == it->y)
    //            return true;
    //    return false;
}

bool Checkers::isValidFieldClicked(int x, int y)
{
    int i = 0;
    if(players[activePlayer]->isWhite())
    {
        for(auto it = validFields.begin(); it != validFields.end(); it++, i++)
            if(it->y < y and it->y + FIELD_SIZE > y and it->x < x and it->x + FIELD_SIZE > x and
               it->y == lastSelectedPawn->getSprite().getPosition().y - FIELD_SIZE and
               (it->x == lastSelectedPawn->getSprite().getPosition().x - FIELD_SIZE or
                it->x == lastSelectedPawn->getSprite().getPosition().x + FIELD_SIZE))
            {
                lastSelectedField = i;
                return true;
            }
    }
    else if(!players[activePlayer]->isWhite())
    {
        for(auto it = validFields.begin(); it != validFields.end(); it++, i++)
            if(it->y < y and it->y + FIELD_SIZE > y and it->x < x and it->x + FIELD_SIZE > x and
               it->y == lastSelectedPawn->getSprite().getPosition().y + FIELD_SIZE and
               (it->x == lastSelectedPawn->getSprite().getPosition().x - FIELD_SIZE or
                it->x == lastSelectedPawn->getSprite().getPosition().x + FIELD_SIZE))
            {
                lastSelectedField = i;
                return true;
            }
    }

    return false;
}

bool Checkers::isPawnOnField(sf::Vector2i field)
{
    for(auto it = pawns.begin(); it != pawns.end(); it++)
        if(it->get()->getSprite().getPosition().x == field.x and
           it->get()->getSprite().getPosition().y == field.y)
            return true;

    return false;
}

bool Checkers::isPawnOnField(Pawn pawn, sf::Vector2i field)
{
    return pawn.getSprite().getPosition().x == field.x and pawn.getSprite().getPosition().y == field.y;
}

void Checkers::closeWindowIfCloseClicked(sf::Event event)
{
    while(window.pollEvent(event))
        if(event.type == sf::Event::Closed)
            window.close();
}

void Checkers::setBoardToPlay()
{
    window.draw(pawns.front()->getSprite());
    window.draw(boardSprite);
    sf::Event event;
    for(auto it = pawns.rbegin(); it != pawns.rend(); it++)
    {
        sf::Vector2f direction = (*it)->normalizedVectorFromMidToStartPos();
        if(direction.x < 0)
        {
            while((*it)->getSprite().getPosition().x - (*it)->startX() > 0)
                movePawnToStartPositions(*it, direction, event);
            it->get()->setAtStartPosition();
        }
        else
        {
            while((*it)->getSprite().getPosition().x - (*it)->startX() < 0)
                movePawnToStartPositions(*it, direction, event);
            it->get()->setAtStartPosition();
        }
    }
}

void Checkers::movePawnToStartPositions(std::shared_ptr<Pawn> pawn, sf::Vector2f direction, sf::Event event)
{
    float speed = 5;
    pawn->getSprite().move(speed * direction);
    window.clear();
    window.draw(boardSprite);
    for(auto it = pawns.begin(); it != pawns.end(); it++)
        window.draw((*it)->getSprite());
    window.draw(pawn->getSprite());
    window.display();
    closeWindowIfCloseClicked(event);
}

void Checkers::setPlayers()
{
    players.push_back(std::unique_ptr<Player>(new Player(validFields, true)));

    if(gameMode == pvp)
        players.push_back(std::unique_ptr<Player>(new Player(validFields, false)));
    else
        players.push_back(std::unique_ptr<Computer>(new Computer(validFields)));
}

void Checkers::setPawns()
{
    int distance = 112 * 3;
    for(int i = 0; i < 4; i++)
    {
        pawns.push_back(
        std::shared_ptr<Pawn>(new Pawn(true, OUTBOARD + distance, BOARD_SIZE - OUTBOARD - FIELD_SIZE)));
        players[0]->pushPawn(pawns.back());
        pawns.push_back(std::shared_ptr<Pawn>(new Pawn(false, OUTBOARD + FIELD_SIZE + distance, OUTBOARD)));
        players[1]->pushPawn(pawns.back());
        pawns.push_back(std::shared_ptr<Pawn>(
        new Pawn(true, OUTBOARD + FIELD_SIZE + distance, BOARD_SIZE - OUTBOARD - FIELD_SIZE * 2)));
        players[0]->pushPawn(pawns.back());
        pawns.push_back(std::shared_ptr<Pawn>(new Pawn(false, OUTBOARD + distance, OUTBOARD + FIELD_SIZE)));
        players[1]->pushPawn(pawns.back());
        pawns.push_back(std::shared_ptr<Pawn>(
        new Pawn(true, OUTBOARD + distance, BOARD_SIZE - OUTBOARD - FIELD_SIZE * 3)));
        players[0]->pushPawn(pawns.back());
        pawns.push_back(std::shared_ptr<Pawn>(
        new Pawn(false, OUTBOARD + FIELD_SIZE + distance, OUTBOARD + 2 * FIELD_SIZE)));
        players[1]->pushPawn(pawns.back());
        distance -= 112;
    }
}

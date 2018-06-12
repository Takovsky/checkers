#include "Checkers.hpp"
#include "GameMode.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

int main()
{
    sf::RenderWindow window(sf::VideoMode(504, 504), "Checkers");
    Checkers checkers(window);

    GameMode mode = pvp;

    checkers.run(mode);

    return 0;
}

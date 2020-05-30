#include <SFML/Graphics.hpp>
#include <vector>
#include <Button.h>
#include <iostream>

#ifndef OANDX_H
#define OANDX_H

/**
*   @brief Small structure used by MinMax algorithm
*/
struct moveScore{
    moveScore(int score){_score = score;}
    sf::Vector2i _bestMove;
    int _score;
};

class OandX
{
    public:
        /**
        *   @param size - size of the board
        *   @param winScore - how many in row is needed to win
        *   @param win - reference to the main window
        *   @param AI_1, AI_2 - if true, player is controlled by MIN-MAX algorithm
        */
        OandX(int size, int winScore, sf::RenderWindow &win, bool AI_1, bool AI_2);

        /**
        *   @brief Whole game process in one place!
        */
        void game();
        void displayScore(sf::Font &f);

    private:
        sf::RenderWindow &_win;
        int _size; // Size of the map
        int sizeButtons; // Size of the _map_button vector
        int _winScore; // Length of the winning line
        bool _AI_X; //
        bool _AI_O; // AI ON/OFF status
        sf::Texture _board;
        sf::Texture _background;
        sf::Texture _O;
        sf::Texture _X;
        sf::Texture _boardX;
        sf::Texture _boardO;
        sf::RectangleShape _bg;
        sf::RectangleShape _cursor;

        std::vector<button> _map_button;
        std::vector<char> _map_char;

        bool _turn = false; // false = O turn, true = X turn
        char _score = ' '; // d - draw, x - x player won, o - o player won, ' ' - game in progress

        ///Game methods
        /**
        *   @param if player = true - X turn, else O turn
        *   @param depth of the algorithm
        *   @param alpha,beta - values used for Alpha-Beta algorithm (ref arg)
        *   @param true if algorithm have to find best possible move in position. (ref arg)
        *   @return Best move for actual player
        */
        int MinMax(bool player, int depth, int alpha, int beta, bool maxPlayer);

        /**
        *   @brief Simple algorithm which works better for bigger size of the board
        *   AI want to build the longest line, and block if enemy is in better situation.
        *   @return Best move in position
        */
        sf::Vector2i myAlgorithm();
        int _depth;
        int _howManyMoves = 0;
        bool _useMinMax = true;
        void turn_X();
        void turn_O();
        void draw(); // draw board
        void checkWin(); //if player = true - X turn, else O turn
                                  //change _score value to d/x/o/
};

#endif // OANDX_H

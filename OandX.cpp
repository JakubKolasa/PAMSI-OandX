#include "OandX.h"
#define INF 999999999

OandX::OandX( int size, int winScore, sf::RenderWindow &win, bool AI_1, bool AI_2): _win(win){
    _size = size;
    _winScore = winScore;
    _AI_O = AI_1;
    _AI_X = AI_2;
    _depth = 12;
    _board.loadFromFile("data/img/board.png");
    _background.loadFromFile("data/img/background.png");
    _X.loadFromFile("data/img/X.png");
    _O.loadFromFile("data/img/O.png");
    _boardO.loadFromFile("data/img/boardO.png");
    _boardX.loadFromFile("data/img/boardX.png");

    _bg.setTexture(&_background);
    _bg.setSize(sf::Vector2f(_win.getSize()));
    _cursor.setSize(sf::Vector2f(30,30));
    _cursor.setOrigin(15,15);

    int length = _size*50;
    int hight  = _size*50;
    int left = (_win.getSize().x-length)/2;
    int top = (_win.getSize().y-hight)/2;
    for(int j=0; j<_size; j++){
        for(int i=0; i<_size; i++){
            _map_button.push_back( button(sf::Vector2f(left+i*50,top+j*50), sf::Vector2f(left+(i+1)*50,top+(j+1)*50), &_board ));
            _map_char.push_back(' ');
        }
    }
}

void OandX::game(){
    sizeButtons = this->_map_button.size();
    while (_win.isOpen())
    {
        sf::Event _event;
        while (_win.pollEvent(_event))
            if (_event.type == sf::Event::Closed) _win.close();

        _win.clear();
        this->draw();
        _win.display();

        if( sizeButtons - _howManyMoves > 11) _useMinMax = false;
        else _useMinMax = true;
        if(_turn) this->turn_X();
        else this->turn_O();
        if(_score != ' '){ return void();}
    }
}

void OandX::draw(){
    if(_turn) _cursor.setTexture(&_X);
    else _cursor.setTexture(&_O);

    _win.draw(this->_bg);
    for(int i = 0; i < sizeButtons; i++ ) _win.draw(_map_button[i].getRectangle());
    _cursor.setPosition(sf::Vector2f(sf::Mouse::getPosition(_win)));
    _win.draw(this->_cursor);
}

void OandX::turn_O(){
    ///AI
    if(_AI_O){
        sf::sleep(sf::seconds(1));
        sf::Vector2i move;
        if(_useMinMax){
            int score = - INF; moveScore bestScore(- INF);
            for(int j = 0; j<_size; j++){
                for(int i = 0; i<_size; i++){
                if(_map_char[j*_size+i] == ' '){
                    _map_char[j*_size+i] = 'o';
                    score = MinMax(true, _depth, - INF, INF, false);
                    if(score>bestScore._score){ bestScore._score = score; bestScore._bestMove.x = i; bestScore._bestMove.y = j; }
                    _map_char[j*_size+i] = ' ';
                    }
                }
            }
            move = bestScore._bestMove;
        }
        else move = myAlgorithm();
        _map_char[move.x + move.y * _size] = 'o';
        _map_button[move.x + move.y * _size].getRectangle().setTexture(&_boardO);
        this->checkWin();
        _howManyMoves++;
        _turn = true;
    }
    ///Player
    else{
    for(int i = 0; i<sizeButtons; i++){
        _map_button[i].isPointed(sf::Mouse::getPosition(_win));
        if(_map_button[i].isPressed(sf::Mouse::getPosition(_win))){
            if(_map_char[i] == ' '){
                _map_char[i] = 'o';
                _map_button[i].getRectangle().setTexture(&_boardO);
                this->checkWin();
                _howManyMoves++;
                _turn = true;
                }
            }
        }
    }
}

void OandX::turn_X(){
    ///AI
    if(_AI_X){
        sf::sleep(sf::seconds(1));
        sf::Vector2i move;
        if(_useMinMax){
            int score = -INF; moveScore bestScore(-INF);
            for(int j = 0; j<_size; j++){
                for(int i = 0; i<_size; i++){
                if(_map_char[j*_size+i] == ' '){
                    _map_char[j*_size+i] = 'x';
                    score = MinMax(false, _depth, -INF, INF, false);
                    if(score>bestScore._score){ bestScore._score = score; bestScore._bestMove.x = i; bestScore._bestMove.y = j; }
                    _map_char[j*_size+i] = ' ';
                    }
                }
            }
            move = bestScore._bestMove;
        }
        else move = myAlgorithm();
        _map_char[move.x + move.y * _size] = 'x';
        _map_button[move.x + move.y * _size].getRectangle().setTexture(&_boardX);
        this->checkWin();
        _howManyMoves++;
        _turn = false;
    }
    ///Player
    else{
    for(int i = 0; i<sizeButtons; i++){
        _map_button[i].isPointed(sf::Mouse::getPosition(_win));
        if(_map_button[i].isPressed(sf::Mouse::getPosition(_win))){
            if(_map_char[i] == ' '){
                _map_char[i] = 'x';
                _map_button[i].getRectangle().setTexture(&_boardX);
                this->checkWin();
                _howManyMoves++;
                _turn = false;
                }
            }
        }
    }
}

void OandX::displayScore(sf::Font &f){
    if(_score == ' ') return void();
    std::string sc;
    if(_score == 'd') sc = "DRAW!";
    if(_score == 'x') sc = "PLAYER X WIN!";
    if(_score == 'o') sc = "PLAYER O WIN!";
    sf::Text T(sc, f); T.setCharacterSize(100);
    T.setPosition(100,600); T.setFillColor(sf::Color::Black);
    draw(); _win.draw(T);
    _win.display();
}

void OandX::checkWin(){
    int winIterator = 0;
    char whoseTurn;

    for (int x = 0; x<2; x++){
    if(x == 0) whoseTurn = 'x';
    if(x == 1) whoseTurn = 'o';

    // Row
    for(int j=0; j<_size; j++){
        winIterator = 0;
        for(int i=0; i<_size; i++){
            if(_map_char[j*_size+i] == whoseTurn) winIterator++;
            else winIterator = 0;
            if(winIterator == _winScore){  _score = whoseTurn; return void();}
        }
    }

    // Column
    for(int i=0; i<_size; i++){
        winIterator = 0;
        for(int j=0; j<_size; j++){
            if(_map_char[j*_size+i] == whoseTurn) winIterator++;
            else winIterator = 0;
            if(winIterator == _winScore){ _score = whoseTurn; return void();}
        }
    }

    // Diagonally left to right
    for( int d = 0; d < _size + _size - 1 ; d++){
        winIterator = 0;
        for( int i = 0, j = _size - d; (i < _size) && (j < _size); i++, j++){
            if ( _map_char[j*_size+i] == whoseTurn){ winIterator++;}
            else winIterator = 0;
            if(winIterator == _winScore){  _score = whoseTurn; return void();}
        }
    }

    // Diagonally right to left
    for( int d = 0; d < _size + _size - 1 ; d++){
        winIterator = 0;
        for( int i = 0, j = d; (i < _size) && (j >= 0); i++, j--){
            if ( _map_char[j*_size+i] == whoseTurn) winIterator++;
            else winIterator = 0;
            if(winIterator == _winScore){ _score = whoseTurn; return void();}
        }
     }
    }
    // Draw
    for(int i=0; i<_size; i++){
        for(int j=0; j<_size; j++){
            if(_map_char[j*_size+i] == ' ') return void();
        }
    }
     _score = 'd'; return void();
}


int OandX::MinMax(bool player, int depth, int alpha, int beta, bool maxPlayer){
    char actuallTurn;
    if(player) actuallTurn = 'x';
    else actuallTurn = 'o';

    if(depth == 0) return 0;
    this->checkWin(); //checkWin change _score value
    if( _score != ' '){
        if( actuallTurn == _score){_score = ' '; return 1;}
        if(( actuallTurn == 'x' && _score == 'o' )||(  actuallTurn == 'o' && _score == 'x' )){_score = ' '; return -1;}
        if( _score == 'd'){_score = ' '; return 0;}
    }

    int mv;
    if(maxPlayer){
        int max = -INF;
        mv = max;
        for(int j = 0; j<_size; j++){
            for(int i = 0; i<_size; i++){
                if(_map_char[j*_size+i] == ' '){
                    _map_char[j*_size+i] = actuallTurn;
                    mv = MinMax(!player, depth-1, alpha, beta, false);
                    if(alpha<mv) alpha = mv;
                    if(alpha>= beta)  break;
                    _map_char[j*_size+i] = ' ';
                }
            }
        }
    }
    else
    {
    int min = INF;
    mv = min;
        for(int j = 0; j<_size; j++){
            for(int i = 0; i<_size; i++){
                if(_map_char[j*_size+i] == ' '){
                    _map_char[j*_size+i] = actuallTurn;
                    mv = MinMax(!player, depth-1, alpha, beta, true);
                    if(beta>mv ) alpha = mv;
                    if(alpha>= beta)  break;
                    _map_char[j*_size+i] = ' ';
                }
            }
        }
    }
    return mv;
}


/**
*   @brief Best move for O and X is the same
*   algorithm works only for square map
*/
sf::Vector2i OandX::myAlgorithm(){
int bestMove[_size][_size]; // = O + X
    int middle; int pointsToAdd;
    if( _size%2 ) middle = _size/2;
    else middle = (_size-1)/2;

    char whoseTurn;
    char actuallTurn;
    if(!_turn) actuallTurn = 'x';
    else actuallTurn = 'o';

    int range;
    int firstPoint = _size;
    int nextPoint = _size*3;
    int winPossibility = firstPoint * pow(nextPoint, _winScore-1);

    /// Set 0
    for(int i = 0; i<_size; i++){
        for(int j = 0; j<_size; j++){
             bestMove[i][j] = 0;
        }
    }

    /// Position on the map.
    /// Middle - more points, corner less etc.
    for(int i = 0; i<_size; i++){
        for(int j = 0; j<_size; j++){
            pointsToAdd = 0;
            if(i < middle) pointsToAdd += i + 1;
            else pointsToAdd += _size - i;
            if(j < middle) pointsToAdd += j + 1;
            else pointsToAdd += _size - j;
            bestMove[i][j] += pointsToAdd;
        }
    }

    /// Combo with others for both players
    for(int a = 0; a<2; a++){
        if(a == 0) whoseTurn = 'x';
        if(a == 1) whoseTurn = 'o';

        // Row
        for(int j=0; j<_size; j++){
            pointsToAdd = 0;
            range = 0;
            for(int i=0; i<_size; i++){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else if(pointsToAdd == winPossibility && whoseTurn == actuallTurn) pointsToAdd = INF;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                    if(pointsToAdd) range++;
                    if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                   }
                }
                else{ pointsToAdd = 0; range = 0;}
            }
        }

        // Row reversed
        for(int j=_size-1; j>=0; j--){
            pointsToAdd = 0;
            range = 0;
            for(int i=_size-1; i>=0; i--){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else if(pointsToAdd == winPossibility && whoseTurn == actuallTurn) pointsToAdd = INF;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                   if(pointsToAdd) range++;
                   if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                   }
                }
                else{ pointsToAdd = 0; range = 0;}
            }
        }

        // Column
        for(int i=0; i<_size; i++){
            pointsToAdd = 0;
            range = 0;
            for(int j=0; j<_size; j++){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else if(pointsToAdd == winPossibility && whoseTurn == actuallTurn) pointsToAdd = INF;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                   if(pointsToAdd) range++;
                   if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                   }
                }
                else{ pointsToAdd = 0; range = 0;}
            }
        }

        // Column reversed
        for(int i=_size-1; i>=0; i--){
            pointsToAdd = 0;
            range = 0;
            for(int j=_size-1; j>=0; j--){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else if(pointsToAdd == winPossibility && whoseTurn == actuallTurn) pointsToAdd = INF;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                   if(pointsToAdd) range++;
                   if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                    }
                }
                else{ pointsToAdd = 0; range = 0;}
            }
        }

        // Diagonally left to right, top to bot
        for( int d = 0; d < _size + _size - 1 ; d++){
            pointsToAdd = 0;
            range = 0;
            for( int i = 0, j = _size - d; (i < _size) && (j < _size); i++, j++){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else if(pointsToAdd == winPossibility && whoseTurn == actuallTurn) pointsToAdd = INF;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                   if(pointsToAdd) range++;
                   if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                   }
                }
                else{ pointsToAdd = 0; range = 0;}
            }
        }

        // Diagonally left to right, bot to top
        for( int d = 0; d < _size + _size - 1 ; d++){
            pointsToAdd = 0;
            range = 0;
            for( int i = _size-1, j = _size -1 ; (i >= 0) && (j >= _size - d); i--, j--){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else if(pointsToAdd == winPossibility && whoseTurn == actuallTurn) pointsToAdd = INF;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                   if(pointsToAdd) range++;
                   if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                   }
                }
                else{ pointsToAdd = 0; range = 0;}
            }
        }

        // Diagonally right to left top to bot
        for( int d = 0; d < _size + _size - 1 ; d++){
            pointsToAdd = 0;
            range = 0;
            for( int i = 0, j = d; (i < _size) && (j >= 0); i++, j--){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else if(pointsToAdd == winPossibility && whoseTurn == actuallTurn) pointsToAdd = INF;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                   if(pointsToAdd) range++;
                   if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                   }
                }
                else{ pointsToAdd = 0; range = 0;}}
        }

        // Diagonally right to left bot to top
        for( int d = 0; d < _size + _size - 1 ; d++){
            pointsToAdd = 0;
            range = 0;
            for( int i = _size -1, j = _size - d; (i >=0) && (j < _size); i--, j++){
                if(_map_char[j*_size+i] == whoseTurn){
                    if(!pointsToAdd) pointsToAdd = firstPoint;
                    else pointsToAdd *= nextPoint;
                }
                else if(_map_char[j*_size+i] == ' '){
                   if(pointsToAdd) range++;
                   if(range < _winScore) {
                        bestMove[i][j] += pointsToAdd - range;
                   }
                }
                else{ pointsToAdd = 0; range = 0;}}
        }
    }

    ///Delete every banned move
    for(int i = 0; i<_size; i++){
        for(int j = 0; j<_size; j++){
            if(_map_char[j*_size+i] != ' '){
                bestMove[i][j] = 0;
            }
        }
    }

    ///,,MinMax"
    /// Best move (best move for u and the biggest loss for enemy
    sf::Vector2i MAX_position(0,0);
    int MAX = 0;
    for(int i = 0; i<_size; i++){
        for(int j = 0; j<_size; j++){
        if(bestMove[i][j] > MAX){
            MAX = bestMove[i][j];
            MAX_position.x = i;
            MAX_position.y = j;
        }
        }
    }
    return MAX_position;
}

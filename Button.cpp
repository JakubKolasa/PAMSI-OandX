#include "Button.h"

button::button(sf::Vector2f topLeft, sf::Vector2f botRight, std::string text, sf::Font *f, sf::Texture *T)
            {
            sf::Vector2f Size = sf::Vector2f(botRight.x-topLeft.x,botRight.y-topLeft.y);
            _table.setSize(Size);
            _table.setPosition(topLeft);
            _table.setTexture(T);

            _textButton = true;
            _text.setFont(*f);
            _text.setString(text);
            _text.setFillColor(sf::Color::Black);
            _text.setCharacterSize(30);
            _text.setOrigin(_text.getLocalBounds().width/2,_text.getLocalBounds().height/2);
            _text.setPosition(sf::Vector2f(topLeft.x+Size.x/2,topLeft.y+Size.y/2));
            }

button::button(sf::Vector2f topLeft, sf::Vector2f botRight,
            sf::Texture *T){
            _table.setSize(sf::Vector2f(botRight.x-topLeft.x,botRight.y-topLeft.y));
            _table.setPosition(topLeft);
            _table.setTexture(T,false);
}

bool button::isPointed(sf::Vector2i point){
    if(_table.getGlobalBounds().contains(point.x, point.y)){
            _table.setFillColor(sf::Color::Green);
            return true;
    }
    _table.setFillColor(sf::Color::White);
    return false;
}

bool button::isPressed(sf::Vector2i point){
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && _table.getGlobalBounds().contains(point.x,point.y)) return true;
    else return false;
}

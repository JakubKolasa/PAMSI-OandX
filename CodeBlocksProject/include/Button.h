#include <SFML/Graphics.hpp>
#include <string>

#ifndef BUTTON_H
#define BUTTON_H

class button
{
    public:
        /**
        *   @brief button with text
        *   @param vector2f topLeft, botRight: corners position
            @param string text: text inside box
        */
        button(sf::Vector2f topLeft, sf::Vector2f botRight,
               std::string text, sf::Font *f, sf::Texture *T);

        /**
        *   @brief button without text
        *   @param vector2f topLeft, botRight: corners position
        */
        button(sf::Vector2f topLeft, sf::Vector2f botRight,
               sf::Texture *T);

        /**
        *   @return true if button is pointed. Changing color of the button.
        */
        bool isPointed(sf::Vector2i _Point);

         /**
        *   @return false if button is pointed
        */
        bool isPressed(sf::Vector2i _Point);
        sf::RectangleShape &getRectangle(){return _table;}
        sf::Text getText(){if (_textButton) return _text;else return sf::Text();}
        void changeText(sf::String newText){_text.setString(newText);}
    private:
        sf::RectangleShape _table;
        sf::Text _text;
        bool _textButton = false;
};

#endif // BUTTON_H

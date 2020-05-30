#include <Button.h>
#include <OandX.h>
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "X&O");
    window.setFramerateLimit(60); window.setActive();


    ///MENU CONFIGURATION
    sf::Font font; font.loadFromFile("data/fonts/Blue.ttf");
    sf::Texture menuButtons; menuButtons.loadFromFile("data/img/menuButton.png");
    sf::Texture AI; AI.loadFromFile("data/img/AI.png");
    sf::Texture PL; PL.loadFromFile("data/img/player.png");
    sf::Texture bg; bg.loadFromFile("data/img/background.png");
    sf::Text p1("PLAYER O", font); p1.setPosition(sf::Vector2f(205,460)); p1.setFillColor(sf::Color::Black);
    sf::Text p2("PLAYER X", font); p2.setPosition(sf::Vector2f(505,460)); p2.setFillColor(sf::Color::Black);

    std::vector<button> buttons;
    buttons.push_back(button(sf::Vector2f(0,0), sf::Vector2f(800,800), &bg));
    buttons.push_back(button(sf::Vector2f(100,100), sf::Vector2f(700,300), "Start game", &font, &menuButtons));
    buttons.push_back(button(sf::Vector2f(200,500),sf::Vector2f(300,600), &AI));
    buttons.push_back(button(sf::Vector2f(500,500), sf::Vector2f(600,600), &PL));
    buttons.push_back(button(sf::Vector2f(100,325), sf::Vector2f(375,425), "Size: 3 x 3", &font, &menuButtons));
    buttons.push_back(button(sf::Vector2f(425,325), sf::Vector2f(700,425), "Winning line: 3", &font, &menuButtons));
    buttons[1].getRectangle().setOutlineThickness(5); buttons[1].getRectangle().setOutlineColor(sf::Color::Black);
    buttons[4].getRectangle().setOutlineThickness(5); buttons[4].getRectangle().setOutlineColor(sf::Color::Black);
    buttons[5].getRectangle().setOutlineThickness(5); buttons[5].getRectangle().setOutlineColor(sf::Color::Black);


    int choseWinLine = 3;
    int  menuChoice = 0;
    bool AI_p1 = true, AI_p2 = false;
    std::string newSize;
    int choseSize = 3;
    std::string winLine;
    OandX *MainGame;

    while (window.isOpen())
    {
        ///Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        ///Draw and display
        for(int i = 0; i<6; i++){
            window.draw(buttons[i].getRectangle());
            window.draw(buttons[i].getText());
            if(i) buttons[i].isPointed(sf::Mouse::getPosition(window));
            if(i) if(buttons[i].isPressed(sf::Mouse::getPosition(window))) menuChoice = i;
            }
        window.draw(p1); window.draw(p2);
        window.display();

        ///Main menu
        switch( menuChoice)
        {
        case 1:
            ///Start game and display score
            buttons.clear();
            MainGame = new OandX(choseSize, choseWinLine, window, AI_p1, AI_p2);
            MainGame->game();
            MainGame->displayScore(font);
            sf::sleep(sf::seconds(5));
            return 0;
        case 2:
            ///Change AI status for player 1
            AI_p1 = !AI_p1;
            if(AI_p1) buttons[2].getRectangle().setTexture(&AI);
            else buttons[2].getRectangle().setTexture(&PL);
            sf::sleep(sf::milliseconds(200));
            break;
        case 3:
            ///Change AI status for player 2
            AI_p2 = !AI_p2;
            if(AI_p2) buttons[3].getRectangle().setTexture(&AI);
            else buttons[3].getRectangle().setTexture(&PL);
            sf::sleep(sf::milliseconds(200));
            break;

        case 4:
            ///Change size of the map
            if(choseSize<15) choseSize ++;
            else choseSize = 3;
            newSize = std::string("Size: ") + std::to_string(choseSize) + std::string(" x ") + std::to_string(choseSize);
            buttons[4].changeText(newSize);
            sf::sleep(sf::milliseconds(200));
            break;
        case 5:
            ///Change size of the winning line
            if(choseWinLine >= choseSize) choseWinLine = 3;
            else choseWinLine++;
            winLine = std::string("Winning line: ") + std::to_string(choseWinLine);
            buttons[5].changeText(winLine);
            sf::sleep(sf::milliseconds(200));
            break;
        }
        menuChoice = 0;
    }
}

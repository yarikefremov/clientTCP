#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>
#include "inetgui.h"

class GUI{
public:
    GUI();
    void init();
private:

    bool enteringName = false;
    bool enteringPassword = false;
    bool isRegisterMode = false;
    bool isChatMode = false;
    bool isAddingUsername = false;
    sf::String nameInput, passwordInput, currentMessage;
    std::vector<sf::Text> chatLog;
    std::vector<std::string> users{"123", "key77", "gregor12", "michael1907"};
    int selectedUserIndex = 1;
    std::vector<sf::Text> userTexts;

    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 600), "TCP CHAT");
    sf::Font font;
    sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);

    sf::Text loginButton = sf::Text("Login", font, 30);
    sf::Text registerButton = sf::Text("Register", font, 30);
    sf::Text okButton = sf::Text("OK", font, 30);
    sf::Text nameText = sf::Text("Name:", font, 25);
    sf::Text passwordText = sf::Text("Password:", font, 25);
    sf::Text nameDisplay =  sf::Text("", font, 25);
    sf::Text passwordDisplay = sf::Text("", font, 25);
    sf::Text messageDisplay = sf::Text("", font, 25);
    sf::Text chatLine = sf::Text("", font, 20);
    sf::Text newDialogText = sf::Text("+", font, 45);

    sf::RectangleShape nameField = sf::RectangleShape(sf::Vector2f(400, 40));
    sf::RectangleShape passwordField = sf::RectangleShape(sf::Vector2f(400, 40));
    sf::RectangleShape messageInputField = sf::RectangleShape(sf::Vector2f(600, 40));

    void loop();
    sf::Event event;
    void mouseEventHandler();
    void inputTextEventHandler();
    void keyEventHandler();
    void draw();
    void redrawUsers();
};
#endif // GUI_H

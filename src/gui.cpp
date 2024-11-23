#include "../include/gui.h"

GUI::GUI(NetworkGUIInterface inetgui){
    this->inetgui = inetgui;
}

void GUI::init(){
    if (!font.loadFromFile("../../../resources/Anonymous_Pro.ttf")) {
        font.loadFromFile("/resources/Anonymous_Pro.ttf");
    }
    window.setFramerateLimit(60);

    nameField.setPosition(windowCenter.x - 50.f, windowCenter.y - 50.f);
    nameField.setFillColor(sf::Color(0xFFFFFFFF));
    nameField.setOutlineThickness(2.f);
    nameField.setOutlineColor(sf::Color(0x2C3987FF));
    passwordField.setPosition(windowCenter.x - 50.f, windowCenter.y + 50.f);
    passwordField.setFillColor(sf::Color(0xFFFFFFFF));
    passwordField.setOutlineThickness(2.f);
    passwordField.setOutlineColor(sf::Color(0x2C3987FF));
    messageInputField.setPosition(windowCenter.x - 300.f, window.getSize().y - 60.f);
    messageInputField.setFillColor(sf::Color(0xFFFFFFFF));
    messageInputField.setOutlineThickness(2.f);
    messageInputField.setOutlineColor(sf::Color(0x2C3987FF));

    loginButton.    setFillColor(sf::Color(0x1B1B1BFF));
    registerButton. setFillColor(sf::Color(0x1B1B1BFF));
    okButton.       setFillColor(sf::Color(0x1B1B1BFF));
    nameText.       setFillColor(sf::Color(0x1B1B1BFF));
    passwordText.   setFillColor(sf::Color(0x1B1B1BFF));
    nameDisplay.    setFillColor(sf::Color(0x1B1B1BFF));
    passwordDisplay.setFillColor(sf::Color(0x1B1B1BFF));
    messageDisplay. setFillColor(sf::Color(0x1B1B1BFF));
    chatLine.       setFillColor(sf::Color(0x1B1B1BFF));
    newDialogText.  setFillColor(sf::Color(0x2C3987FF));
    loginButton.    setPosition(windowCenter.x - 150.f, windowCenter.y - 150.f);
    registerButton. setPosition(windowCenter.x + 50.f, windowCenter.y - 150.f);
    okButton.       setPosition(windowCenter.x - 50.f, windowCenter.y + 150.f);
    nameText.       setPosition(windowCenter.x - 250.f, windowCenter.y - 50.f);
    passwordText.   setPosition(windowCenter.x - 250.f, windowCenter.y + 50.f);
    nameDisplay.    setPosition(windowCenter.x - 45.f, windowCenter.y - 45.f);
    passwordDisplay.setPosition(windowCenter.x - 45.f, windowCenter.y + 55.f);
    messageDisplay. setPosition(messageInputField.getPosition().x + 10.f, messageInputField.getPosition().y + 5.f);
    newDialogText.  setPosition(messageInputField.getPosition().x - 30.f, messageInputField.getPosition().y - messageInputField.getSize().y/2+6);

    redrawUsers();

    loop();
}

void GUI::loop(){

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) mouseEventHandler();
            if (event.type == sf::Event::TextEntered)        inputTextEventHandler();
            if (event.type == sf::Event::KeyPressed) keyEventHandler();
        }
        //inetgui.getInputMsg();
        draw();
    }
}

void GUI::mouseEventHandler(){
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (loginButton.getGlobalBounds().contains(mousePos)) {
        enteringName = true;
        enteringPassword = false;
        isRegisterMode = false; // Переходим в режим авторизации
    }

    if (registerButton.getGlobalBounds().contains(mousePos)) {
        enteringName = true;
        enteringPassword = false;
        isRegisterMode = true; // Переходим в режим регистрации
    }

    if (okButton.getGlobalBounds().contains(mousePos)) {
        //inetgui.setAuth();
        //inetgui.getAccept();
        isChatMode = true; // Переходим в режим чата
    }

    if (nameField.getGlobalBounds().contains(mousePos)) {
        enteringName = true;
        enteringPassword = false;
    }

    if (passwordField.getGlobalBounds().contains(mousePos)) {
        enteringName = false;
        enteringPassword = true;
    }

    if(newDialogText.getGlobalBounds().contains(mousePos)) {
        isAddingUsername = isAddingUsername ? 0 : 1 ;
    }
}

void GUI::inputTextEventHandler(){
    char enteredChar = static_cast<char>(event.text.unicode);
    if (!isChatMode) {

        // Разрешаем только ASCII символы (0-127), исключая пробелы, и обрабатываем Backspace
        if (enteredChar >= 33 && enteredChar <= 126) { // Исключаем пробел (32)
            if (enteringName && nameInput.getSize() < 32) {
                nameInput += enteredChar;
            } else if (enteringPassword && passwordInput.getSize() < 32) {
                passwordInput += enteredChar;
            }
        } else if (event.text.unicode == 8) { // Backspace
            if (enteringName && !nameInput.isEmpty()) {
                nameInput.erase(nameInput.getSize() - 1);
            } else if (enteringPassword && !passwordInput.isEmpty()) {
                passwordInput.erase(passwordInput.getSize() - 1);
            }
        }

        nameDisplay.setString(nameInput);
        passwordDisplay.setString(passwordInput);
    }
    else{
        // Разрешаем только ASCII символы (0-127), включая пробелы, и обрабатываем Backspace
        if ((enteredChar >= 32 && enteredChar <= 126) && currentMessage.getSize() < 99) { // 99 символов максимум
            currentMessage += enteredChar;
        } else if (event.text.unicode == 8 && !currentMessage.isEmpty()) { // Backspace
            currentMessage.erase(currentMessage.getSize() - 1);
        }
        messageDisplay.setString(currentMessage);
    }
}

void GUI::keyEventHandler(){
    if (event.key.code == sf::Keyboard::Up) {
        selectedUserIndex = (selectedUserIndex - 1 + users.size()) % users.size();
    }
    else if (event.key.code == sf::Keyboard::Down)
        selectedUserIndex = (selectedUserIndex + 1) % users.size();
    else if (event.key.code == sf::Keyboard::Enter)
        if(!currentMessage.isEmpty()) {
            if(isAddingUsername){
                users.push_back(currentMessage.toAnsiString());
                currentMessage.clear();
                messageDisplay.setString(currentMessage);
                isAddingUsername = 0;
                return redrawUsers();
            }
            //inetgui.setOutputMsg();
            sf::Text newChatLine(nameInput+": "+currentMessage, font, 20);
            newChatLine.setFillColor(sf::Color(0x1B1B1BFF));
            newChatLine.setPosition(20.f, 20.f + chatLog.size() * 25.f); // Располагаем сообщения друг под другом
            chatLog.push_back(newChatLine);
            currentMessage.clear(); // Очищаем поле ввода
            messageDisplay.setString(currentMessage);
        }
}


void GUI::draw(){

    if (isRegisterMode) {
        registerButton.setFillColor(sf::Color(0x2C3987FF));
        loginButton.setFillColor(sf::Color(0x1B1B1BFF));
    } else {
        loginButton.setFillColor(sf::Color(0x2C3987FF));
        registerButton.setFillColor(sf::Color(0x1B1B1BFF));
    }
    newDialogText.setFillColor(sf::Color(isAddingUsername ? 0x40A179FF: 0x2C3987FF));
    for (size_t i = 0; i < userTexts.size(); ++i) {
        if (i == selectedUserIndex) {
            userTexts[i].setFillColor(sf::Color(0x2C3987FF)); // Выбранный пользователь
        } else {
            userTexts[i].setFillColor(sf::Color(0x1B1B1BFF));
        }
    }

    window.clear(sf::Color(0xEDF0F6FF));

    if (!isChatMode) {
        window.draw(loginButton);
        window.draw(registerButton);
        window.draw(nameField);
        window.draw(passwordField);
        window.draw(nameText);
        window.draw(passwordText);
        window.draw(nameDisplay);
        window.draw(passwordDisplay);
        window.draw(okButton);
    } else {
        // Отображаем лог сообщений
        for (const auto& text : userTexts) {
            window.draw(text);
        }
        for (const auto& line : chatLog) {
            window.draw(line);
        }
        window.draw(messageInputField);
        window.draw(messageDisplay);
        window.draw(newDialogText);
    }

    window.display();
}

void GUI::redrawUsers(){
    userTexts.clear();
    userTexts.reserve(users.size());
    for (size_t i = 0; i < users.size(); ++i) {
        userTexts.push_back(sf::Text(users[i], font, 20));
        userTexts[i].setFont(font);
        userTexts[i].setString(users[i]);
        userTexts[i].setCharacterSize(24);
        userTexts[i].setPosition(100.f, 100.f + i * 40.f);
    }
}

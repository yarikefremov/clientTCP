#include "../include/gui.h"

GUI::GUI(NetworkGUIInterface* inetgui){
    this->inetgui = inetgui;
}

void GUI::init(){
    window.setActive();
    if (!font.loadFromFile("../../../resources/Anonymous_Pro.ttf")) {
        if(!font.loadFromFile("/resources/Anonymous_Pro.ttf")) font.loadFromFile("Anonymous_Pro.ttf");
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
                inetgui->netstop();
                netThread->join();
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) mouseEventHandler();
            if (event.type == sf::Event::TextEntered)        inputTextEventHandler();
            if (event.type == sf::Event::KeyPressed) keyEventHandler();
        }
        if(isChatMode && inetgui->flags[0])
            if(inetgui->getInputMsg(this->newMsg)){
                auto it = std::find(users.begin(), users.end(), std::string(newMsg.srcname));
                if(it != users.end()){
                    //Нашли, добавляем сообщение к этому пользователю
                    sf::Text msg(sf::String(newMsg.srcname)+": "+sf::String(newMsg.msg), font, 14);
                    msg.setFillColor(sf::Color(0x1B1B1BFF));
                    msg.setPosition(20.f, 20.f + chatLog.log[selectedUserIndex].size() * 25.f); // Располагаем сообщения друг под другом
                    chatLog.log[it - users.begin()].push_back(msg);
                    redrawUsers();
                }
                else{
                    //Добавляем пользователя + к нему сообщение
                    users.push_back(newMsg.srcname);
                    sf::Text msg(sf::String(newMsg.srcname)+": "+sf::String(newMsg.msg), font, 14);
                    msg.setFillColor(sf::Color(0x1B1B1BFF));
                    msg.setPosition(20.f, 20.f); // Располагаем сообщения друг под другом
                    chatLog.log.push_back({msg});
                    redrawUsers();
                }
            }
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
        if(nameInput.getSize() == 0 || passwordInput.getSize() == 0) return;
        auth.loginflag = !isRegisterMode;
        memcpy(auth.username, nameInput.toAnsiString().c_str(), 33);
        memcpy(auth.password, passwordInput.toAnsiString().c_str(), 33);
        while(!inetgui->setAuth(auth)) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        while(!inetgui->getAccept(acc)) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        memcpy(ownMsg.srcname, auth.username, 33);
        isChatMode = acc.ans; // Переходим в режим чата
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
        selectedUserIndex = (selectedUserIndex - 1) % users.size(); //0-0 1-1 2-2 3-3 4-4 5-0
    }
    else if (event.key.code == sf::Keyboard::Down)
        selectedUserIndex = (selectedUserIndex + 1) % users.size();
    else if (event.key.code == sf::Keyboard::Enter)
        if(!currentMessage.isEmpty()) {
            if(isAddingUsername){
                if(std::find(users.begin(), users.end(), currentMessage.toAnsiString()) == users.end()){
                    users.push_back(currentMessage.toAnsiString());
                    chatLog.log.push_back({sf::Text()});
                }
                currentMessage.clear();
                messageDisplay.setString(currentMessage);
                isAddingUsername = 0;
                return redrawUsers();
            }

            memcpy(ownMsg.dstname, users[selectedUserIndex].c_str(), 33);
            memcpy(ownMsg.msg, currentMessage.toAnsiString().c_str(), 100);
            inetgui->setOutputMsg(ownMsg);
            sf::Text newChatLine(nameInput+": "+currentMessage, font, 14);
            newChatLine.setFillColor(sf::Color(0x1B1B1BFF));
            newChatLine.setPosition(20.f, 20.f + chatLog.log[selectedUserIndex].size() * 25.f); // Располагаем сообщения друг под другом
            chatLog.log[selectedUserIndex].push_back(newChatLine);
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
        for (const auto& line : chatLog.log[selectedUserIndex % users.size()]) {
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
        userTexts.push_back(sf::Text(users[i], font, 18));
        userTexts[i].setFont(font);
        userTexts[i].setString(users[i]);
        userTexts[i].setPosition(600.f, 20.f + i * 20.f);
    }
}

#ifndef INETGUI_H
#define INETGUI_H
#include <mutex>
#include "pckg.h"
// #include <iostream>

// Флаги пакетов, 1 - пакет доступен для чтения, 0 - пакет доступен для записи
#define FLAG_INPUT_MSG 0      // Входящее сообщение
#define FLAG_OUTPUT_MSG 1     // Исходящее сообщение
#define FLAG_AUTH_REQUEST 2   // Авторизация
#define FLAG_ACCEPT_REQUEST 3 // Ответ от сервера

class NetworkGUIInterface{
private:
    std::mutex m_mutex;
    msgpckg sending_msg;     // gui -> net
    msgpckg received_msg;    // net -> gui
    authpckg sending_req;    // gui -> net
    acceptpckg accepted_req; // net -> gui
    bool net_stop = 0; // Флаг для отановки tcpclient, 0 - работа, 1 -
                       // остановка
  public:
    NetworkGUIInterface();
    
    //server -> net -> gui
    bool getInputMsg(msgpckg& dst);
    bool setInputMsg(msgpckg& data);
    bool getAccept(acceptpckg& dst);
    bool setAccept(acceptpckg& data);

    //gui -> net -> server
    bool setAuth(authpckg& data);
    bool getAuth(authpckg& dst);
    bool setOutputMsg(msgpckg& data);
    bool getOutputMsg(msgpckg& dst);

    // Функция, которую должен использовать gui
    void netstop();

    // Функция, которую должен использовать tcpclient
    bool isNetRunning ();

    // Флаги пакетов
    bool flags[4]{0, 0, 0, 0};
};
#endif

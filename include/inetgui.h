#ifndef INETGUI_H
#define INETGUI_H
#include <mutex>
#include "pckg.h"
#include <iostream>
class NetworkGUIInterface{
private:
    std::mutex m_mutex;
    msgpckg sending_msg;        //gui -> net
    msgpckg received_msg;       //net -> gui
    authpckg sending_req;       //gui -> net
    acceptpckg accepted_req;    //net -> gui
    bool net_stop = 0;
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

    void netstop();
    bool isNetRunning();

    bool flags[4]{0, 0, 0, 0};
};
#endif

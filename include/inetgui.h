#ifndef INETGUI_H
#define INETGUI_H
#include <mutex>
#include "pckg.h"
class NetworkGUIInterface{
private:
    std::mutex m_mutex;
    msgpckg sending_msg;        //gui -> net
    msgpckg received_msg;       //net -> gui
    authpckg sending_req;       //gui -> net
    acceptpckg accepted_req;    //net -> gui
    bool flags[4]{0, 0, 0, 0};
public:
    NetworkGUIInterface();
    //server -> net -> gui
    msgpckg* getInputMsg() { return &received_msg;}
    void  setInputMsg();
    acceptpckg* getAccept(){ return &accepted_req;}
    void setAccept();

    //gui -> net -> server
    void sendAuth();
    void sendMsg();
};
#endif

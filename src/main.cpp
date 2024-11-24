#include "../include/gui.h"
#include "../include/tcpclient.h"
#include "../include/inetgui.h"

void netThreadFunc(TCPClient* client){
    client->threadRecv();
}

int main(int argc, char** argv){
    NetworkGUIInterface inetgui;
    TCPClient tcp(&inetgui, argc > 1? argv[1]:"127.0.0.1");
    GUI gui(&inetgui);
    std::thread netThread(netThreadFunc, &tcp);
    gui.netThread = &netThread;
    gui.init();
    return 0;
}

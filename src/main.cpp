#include "../include/gui.h"
#include "../include/tcpclient.h"
#include "../include/inetgui.h"

void netThreadFunc(TCPClient* client){
    client->threadRecv();
}

int main() {
    NetworkGUIInterface inetgui;
    //msgpckg msg{1u, "meepo", "walter", 0, "can i have some cookies from u?"};
    //acceptpckg acc{2u, 1, 1};
    //inetgui.setInputMsg(msg);
    //inetgui.setAccept(acc);
    TCPClient tcp(&inetgui);
    GUI gui(&inetgui);
    std::thread netThread(netThreadFunc, &tcp);
    gui.netThread = &netThread;
    gui.init();
    return 0;
}

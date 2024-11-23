#include "../include/gui.h"
#include "../include/tcpclient.h"
#include "../include/inetgui.h"

int main() {
    NetworkGUIInterface inetgui;
    TCPClient tcp(&inetgui);
    //GUI gui(&inetgui);
    //std::thread netThread(netThreadFunc, gui.netclient);
    //gui.init();
    return 0;
}

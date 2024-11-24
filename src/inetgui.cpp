#include "../include/inetgui.h"

NetworkGUIInterface::NetworkGUIInterface(){

}

bool NetworkGUIInterface::setAuth(authpckg& data){
    std::cout<<"setAuth"<<std::endl;
    if(flags[2]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    this->sending_req = data;
    flags[2] = 1;
    return 1;
}

bool NetworkGUIInterface::getAuth(authpckg& dst){
    std::cout<<"getAuth"<<std::endl;
    if(!flags[2]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    dst = this->sending_req;
    flags[2] = 0;
    return 1;
}

bool NetworkGUIInterface::setAccept(acceptpckg& data){
    std::cout<<"setAccept"<<std::endl;
    if(flags[3]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    this->accepted_req = data;
    flags[3] = 1;
    return 1;
}

bool NetworkGUIInterface::getAccept(acceptpckg& dst){
    std::cout<<"getAccept"<<std::endl;
    if(!flags[3]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    dst = this->accepted_req;
    flags[3] = 0;
    return 1;
}

bool NetworkGUIInterface::setInputMsg(msgpckg& data){
    std::cout<<"setInputMsg"<<std::endl;
    if(flags[0]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    this->sending_msg = data;
    flags[0] = 1;
    return 1;
}

bool NetworkGUIInterface::getInputMsg(msgpckg& dst){
    std::cout<<"getInputMsg"<<std::endl;
    if(!flags[0]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    dst = this->sending_msg;
    flags[0] = 0;
    return 1;
}

bool NetworkGUIInterface::setOutputMsg(msgpckg& data){
    std::cout<<"setOutputMsg"<<std::endl;
    if(flags[1]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    this->received_msg = data;
    flags[1] = 1;
    return 1;
}

bool NetworkGUIInterface::getOutputMsg(msgpckg& dst){
    std::cout<<"getOutputMsg"<<std::endl;
    if(!flags[1]) return 0;
    std::lock_guard <std::mutex> guard(m_mutex);
    dst = this->received_msg;
    flags[1] = 0;
    return 1;
}

void NetworkGUIInterface::netstop(){
    std::lock_guard <std::mutex> guard(m_mutex);
    net_stop = 1;
}

bool NetworkGUIInterface::isNetRunning(){
    return !net_stop;
}

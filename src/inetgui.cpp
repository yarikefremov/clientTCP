#include "../include/inetgui.h"

NetworkGUIInterface::NetworkGUIInterface(){

}

bool NetworkGUIInterface::setAuth(authpckg& data){
  // std::cout<<"setAuth"<<std::endl;
  if (flags[FLAG_AUTH_REQUEST])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  this->sending_req = data;
  flags[FLAG_AUTH_REQUEST] = 1;
  return 1;
}

bool NetworkGUIInterface::getAuth(authpckg& dst){
  // std::cout<<"getAuth"<<std::endl;
  if (!flags[FLAG_AUTH_REQUEST])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  dst = this->sending_req;
  flags[FLAG_AUTH_REQUEST] = 0;
  return 1;
}

bool NetworkGUIInterface::setAccept(acceptpckg& data){
  // std::cout<<"setAccept"<<std::endl;
  if (flags[FLAG_ACCEPT_REQUEST])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  this->accepted_req = data;
  flags[FLAG_ACCEPT_REQUEST] = 1;
  return 1;
}

bool NetworkGUIInterface::getAccept(acceptpckg& dst){
  // std::cout<<"getAccept"<<std::endl;
  if (!flags[FLAG_ACCEPT_REQUEST])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  dst = this->accepted_req;
  flags[FLAG_ACCEPT_REQUEST] = 0;
  return 1;
}

bool NetworkGUIInterface::setInputMsg(msgpckg& data){
  // std::cout<<"setInputMsg"<<std::endl;
  if (flags[FLAG_INPUT_MSG])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  this->sending_msg = data;
  flags[FLAG_INPUT_MSG] = 1;
  return 1;
}

bool NetworkGUIInterface::getInputMsg(msgpckg& dst){
  // std::cout<<"getInputMsg"<<std::endl;
  if (!flags[FLAG_INPUT_MSG])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  dst = this->sending_msg;
  flags[FLAG_INPUT_MSG] = 0;
  return 1;
}

bool NetworkGUIInterface::setOutputMsg(msgpckg& data){
  // std::cout<<"setOutputMsg"<<std::endl;
  if (flags[FLAG_OUTPUT_MSG])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  this->received_msg = data;
  flags[FLAG_OUTPUT_MSG] = 1;
  return 1;
}

bool NetworkGUIInterface::getOutputMsg(msgpckg& dst){
  // std::cout<<"getOutputMsg"<<std::endl;
  if (!flags[FLAG_OUTPUT_MSG])
    return 0;
  std::lock_guard<std::mutex> guard (m_mutex);
  dst = this->received_msg;
  flags[FLAG_OUTPUT_MSG] = 0;
  return 1;
}

void NetworkGUIInterface::netstop(){
    std::lock_guard <std::mutex> guard(m_mutex);
    net_stop = 1;
}

bool NetworkGUIInterface::isNetRunning(){
    return !net_stop;
}

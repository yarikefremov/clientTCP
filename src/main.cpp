#include "../include/gui.h"
#include "../include/tcpclient.h"
#include "../include/inetgui.h"

// Функция для запуска потока
void
netThreadFunc (TCPClient *client)
{
  client->threadRecv ();
}

int
main (int argc, char **argv)
{
  NetworkGUIInterface inetgui;
  std::string server_ip = argc > 1 ? argv[1] : "127.0.0.1";
  TCPClient tcp (&inetgui, server_ip);
  GUI gui (&inetgui);

  // Бесконечный цикл №1
  // .join() производится в gui
  std::thread net_thread (netThreadFunc, &tcp);

  gui.m_net_thread_ptr = &net_thread;

  // Бесконечный цикл №2
  gui.init ();

  return 0;
}

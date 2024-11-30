#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <WS2tcpip.h>
#include "pckg.h"
#include "inetgui.h"

class TCPClient
{
public:
  TCPClient (NetworkGUIInterface *inetgui_ptr, std::string &ip_address);
  ~TCPClient ();

  void threadRecv ();

  // Функции работы с пакетами
  void auth ();

private:
  // Функции работы с сокетами, общением по TCP

  // Тут выставляются всякие служебные данные (версия и
  // WSAdata)
  bool initWinsock ();

  // Подключение подключение сокета к какому-то адресу (IPv4) + порту на компе
  void createSocket ();

  // Подключение сокета к серверу
  bool connectSock ();

  // Указатель на объект класса, который + использует gui (функции класса
  // thread-save)
  NetworkGUIInterface *m_inetgui_ptr;

  // IP и порт сервера, IP инициализируется в конструкторе, не очень правильно
  // брать порты 50000+ для сервера
  std::string m_server_ip;
  unsigned m_server_port = 54010;

  // Winsock2 информация + сокет, таймаут для recv функции, иначе он просто
  // висеть там будет
  sockaddr_in m_hint;
  DWORD m_timeout = 350;
  SOCKET m_server_socket;

  // Буферочек для пакетов, неизвестно какой пакет прислали, прежде чем нам
  // узнать байты надо куда-то сложить, размер равен размеру самого большого
  // пакета
  char m_buf[sizeof (msgpckg)];

  // Флаг присоединения к чату. Не имеет смысла ждать от сервера сообщений,
  // если мы на нем не авторизировались, сервер даже не знает нашего имени,
  // поэтому сообщения прислать не сможет
  bool m_join_chat = 0;

  // Пакетики, сюда складываем из буфера все, а потом отправляем по ссылке в
  // inetgui, ставим флаг, что пакет доступен для чтения и gui его читает
  acceptpckg m_accept_pack;
  authpckg m_reg_pack;
  msgpckg m_msg_pack;
};

#endif // TCPCLIENT_H

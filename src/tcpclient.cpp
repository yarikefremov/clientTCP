#include "../include/tcpclient.h"
#include <chrono>
#include <thread>

TCPClient::TCPClient (NetworkGUIInterface *inetgui_ptr,
                      std::string &ip_address)
    : m_server_ip (ip_address)
{
  this->m_inetgui_ptr = inetgui_ptr;
  if (initWinsock ())
    {
      createSocket ();
      connectSock ();
    }
}

TCPClient::~TCPClient ()
{
  closesocket (m_server_socket);
  WSACleanup ();
}

bool
TCPClient::initWinsock ()
{

  WSADATA data;
  WORD ver = MAKEWORD (2, 2);
  int wsResult = WSAStartup (ver, &data);
  if (wsResult != 0)
    {
      // std::cout << "Error: can't start Winsock." << std::endl;
      return 0;
    }
  return 1;
}

void
TCPClient::createSocket ()
{

  // сокет типа AF_INET, он же IPv4, SOCK_STREAM это для TCP, SOCK_DGRAM для
  // UDP
  m_server_socket = socket (AF_INET, SOCK_STREAM, 0);
  if (m_server_socket == INVALID_SOCKET)
    {
      // std::cout << "Error: can't create socket." << std::endl;
      WSACleanup ();
    }

  // Честно говоря не понимаю, что тут творится, записывается информация о
  // сокете в какую-то структуру
  // В документации написано, что в sockaddr_in записывается порт и тип IP
  m_hint.sin_family = AF_INET;
  m_hint.sin_port = htons (m_server_port);

  // Тут у нас преобразуется адрес сервера в числовую форму и записывается в
  // тот же sockaddr_in
  inet_pton (AF_INET, m_server_ip.c_str (), &m_hint.sin_addr);
}

void
TCPClient::threadRecv ()
{

  // Пробуем авторизироваться
  while (!m_join_chat && m_inetgui_ptr->isNetRunning ())
    {

      // Если стоит флажок, то gui уже что-то записал в структуру и можно
      // забрать это
      if (m_inetgui_ptr->flags[FLAG_AUTH_REQUEST])

        // Если получается забрать, то отправляем серверу пакетик
        if (m_inetgui_ptr->getAuth (m_reg_pack))
          {
            auth ();
          }
    }

  // Пока пользователь не выключит окно sfml, netstop() не выполнится и
  // tcpclient будет в этом цикле, после авторизации
  while (m_inetgui_ptr->isNetRunning ())
    {

      // Если gui записал сообщение от пользователя, значит мы его отправляем
      if (m_inetgui_ptr->flags[FLAG_OUTPUT_MSG])
        {
          if (m_inetgui_ptr->getOutputMsg (m_msg_pack))
            {
              send (m_server_socket, (const char *)(&m_msg_pack),
                    sizeof (msgpckg), 0);
            }
        }

      // Получаем в цикле сообщения, таймаут на recv находится в .h файле
      int bytesReceived = recv (m_server_socket, m_buf, sizeof (m_buf), 0);

      // std::cout << "bytesReceived: " << bytesReceived << std::endl;

      // If client disconnects, bytesReceived = 0; if error, bytesReceived =
      // -1;
      if (bytesReceived > 0)
        {

          // Получили новое сообщение
          if (m_buf[0] == FLAG_MSG_PACK)
            {

              // Копируем его из буфера в пакет
              memcpy (&m_msg_pack, m_buf, sizeof (msgpckg));

              // Отправляем в inetgui
              m_inetgui_ptr->setInputMsg (m_msg_pack);
            }
        }
    }
}

bool
TCPClient::connectSock ()
{

  // If !initWinsock -> return false.
  // Пробуем подключиться к серверу
  int connResult
      = connect (m_server_socket, (sockaddr *)&m_hint, sizeof (m_hint));

  // Не получилось
  if (connResult == SOCKET_ERROR)
    {
      // std::cout << "Error: can't connect to server." << std::endl;

      // Закрываем все
      closesocket (m_server_socket);
      WSACleanup ();
      return 0;
    }

  // Выставляем таймаут для recv, так как иначе программа долго будет висеть в
  // recv
  setsockopt (m_server_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_timeout,
              sizeof (m_timeout));
  return 1;
}

void
TCPClient::auth ()
{
  // Отправляем пакет регистрации
  send (m_server_socket, (const char *)(&m_reg_pack), sizeof (authpckg), 0);

  // Ждем ответа от сервера, если не получаем, то выкидываемся обратно в цикл
  if (recv (m_server_socket, m_buf, sizeof (m_buf), 0) == -1)
    return;

  // Получив ответ, радостно заполняем структуру
  memcpy (&m_accept_pack, m_buf, sizeof (m_accept_pack));

  // Сервер может не принять нас к себе, поэтому его ответ для нас закон
  m_join_chat = m_accept_pack.ans;

  // Забиваем структуру в inetgui
  m_inetgui_ptr->setAccept (m_accept_pack);
}

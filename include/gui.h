#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>
#include "inetgui.h"
#include <chrono>
#include <thread>

// Окно
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TITLE "TCP CHAT"
#define CENTER_X WINDOW_WIDTH / 2
#define CENTER_Y WINDOW_HEIGHT / 2

// Шрифт
#define FONT_SIZE_HUGE 45
#define FONT_SIZE_BIG 40
#define FONT_SIZE_MIDDLE 30
#define FONT_SIZE_SMALL 25
#define FONT_SIZE_TINY 20
#define FONT_SIZE_SMALLEST 14

// Цвета
#define WHITE_COLOR sf::Color (0xFFFFFFFF)
#define BLUE_COLOR sf::Color (0x2C3987FF)
#define BLACK_COLOR sf::Color (0x1B1B1BFF)
#define GREEN_COLOR sf::Color (0x40A179FF)
#define GRAY_COLOR sf::Color (0xEDF0F6FF)
#define RED_COLOR sf::Color (0xFF3333FF)

class GUI{
public:
  GUI (NetworkGUIInterface *inetgui_ptr);
  void init ();
  std::thread *m_net_thread_ptr;

private:
  // SFML
  sf::RenderWindow m_window = sf::RenderWindow (
      sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), // размеры окна
      TITLE,                                       // название окна
      sf::Style::Titlebar | sf::Style::Close); // флаги (сейчас стоит, чтобы
                                               // нельзя было менять размер)
  sf::Event m_event;
  sf::Font m_font;

  // Флаги состояния
  bool m_is_entering_name
      = 0; // 1 - пользователь вводит имя, 0 - пользователь вводит пароль
  bool m_is_register_mode
      = 0; // 1 - пользователь регистрируется, 0 - пользователь логинится
  bool m_is_chat_mode = 0; // 1 - режим чата, 0 - режим регистрации
  bool m_is_adding_username = 0; // 1 - режим чата, 0 - режим регистрации
  bool m_is_error_occured = 0; // 1 - есть ошибки, 0 - ошибок нет
  // Network
  NetworkGUIInterface *m_inetgui_ptr;

  // Data
  std::vector<std::vector<sf::Text> > m_chat_log{
    {}
  }; // Двумерный массив, который хранит историю чата для каждого пользователя
  std::vector<std::string> m_users{
    "Favourites"
  }; // Имена пользователей (С++), Favourites - костыль, чтобы длина вектора
     // никогда не была нулевой

  // ListModel
  int m_selected_index = 0; // Текущий выбранный пользователь
  std::vector<sf::Text> m_user_name_text; // Имена пользователей (SFML)

  // Вводимые данные
  sf::String m_name_input, // Введенное имя
      m_password_input,    // Введенный пароль
      m_current_message;   // Введенное сообщение
  char m_entered_char;     // Введенный символ

  // Text
  // Label   - выводит текст
  // Display - выводит редактируемый текст
  // Button  - выводит текст, может нажиматься

  // ОКНО РЕГИСТРАЦИИ
  sf::Text m_login_button = sf::Text ("Login", m_font, FONT_SIZE_MIDDLE);
  sf::Text m_register_button = sf::Text ("Register", m_font, FONT_SIZE_MIDDLE);
  sf::Text m_ok_button = sf::Text ("OK", m_font, FONT_SIZE_MIDDLE);
  sf::Text m_name_label = sf::Text ("Name:", m_font, FONT_SIZE_SMALL);
  sf::Text m_password_label = sf::Text ("Password:", m_font, FONT_SIZE_SMALL);
  sf::Text m_name_display = sf::Text ("", m_font, FONT_SIZE_SMALL);
  sf::Text m_password_display = sf::Text ("", m_font, FONT_SIZE_SMALL);
  sf::Text m_error_label
      = sf::Text ("Connection Error ", m_font, FONT_SIZE_TINY);
  //================

  // ОКНО ЧАТА
  sf::Text m_message_label = sf::Text (
      "", m_font, FONT_SIZE_SMALLEST); // Отображается сверху в чате, пихается
                                       // в конец вектора
  sf::Text m_message_display = sf::Text (
      "", m_font, FONT_SIZE_SMALL); // отображается в прямоугольнике внизу
  sf::Text m_new_dialog_sign
      = sf::Text ("+", m_font, FONT_SIZE_HUGE); // плюсик слева
  //================

  // Прямоугольники для ввода
  sf::RectangleShape m_name_field
      = sf::RectangleShape (sf::Vector2f (400, 40));
  sf::RectangleShape m_password_field
      = sf::RectangleShape (sf::Vector2f (400, 40));
  sf::RectangleShape m_message_input_field
      = sf::RectangleShape (sf::Vector2f (600, 40));

  // Функции
  void loop ();
  void mouseEventHandler ();
  void inputTextEventHandler ();
  void keyEventHandler ();
  void draw ();
  void netHandle ();
  void redrawUsers (); // После добавления нового пользователя обязательно
                       // нужно перерисовывать ListModel

  // Пакеты
  msgpckg m_input_msg_pack; // Входящее сообщение
  msgpckg m_output_msg;     // Исходящее сообщение
  authpckg m_auth_pack;     // Пакет для регистрации
  acceptpckg m_accept_pack; // Ответ от сервера
};
#endif // GUI_H

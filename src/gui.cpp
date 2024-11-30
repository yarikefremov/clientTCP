#include "../include/gui.h"

GUI::GUI (NetworkGUIInterface *inetgui_ptr) { m_inetgui_ptr = inetgui_ptr; }

void GUI::init(){

  // Загрузка шрифта
  // 1 и 2 для разработки, 3 - для выпуска
  // if (!m_font.loadFromFile ("../../../resources/Anonymous_Pro.ttf"))
  // if (!m_font.loadFromFile ("/resources/Anonymous_Pro.ttf"))
  m_font.loadFromFile ("Anonymous_Pro.ttf");

  // ФПС
  m_window.setFramerateLimit (30);

  // Цвет, позиция, толщина линий прямоугольников
  m_name_field.setPosition (CENTER_X - 50.f, CENTER_Y - 50.f);
  m_name_field.setFillColor (WHITE_COLOR);
  m_name_field.setOutlineThickness (2.f);
  m_name_field.setOutlineColor (BLUE_COLOR);

  m_password_field.setPosition (CENTER_X - 50.f, CENTER_Y + 50.f);
  m_password_field.setFillColor (WHITE_COLOR);
  m_password_field.setOutlineThickness (2.f);
  m_password_field.setOutlineColor (BLUE_COLOR);

  m_message_input_field.setPosition (CENTER_X - 300.f, WINDOW_HEIGHT - 60.f);
  m_message_input_field.setFillColor (WHITE_COLOR);
  m_message_input_field.setOutlineThickness (2.f);
  m_message_input_field.setOutlineColor (BLUE_COLOR);

  // Цвет текста и расположение всех Text
  m_login_button.setFillColor (BLACK_COLOR);
  m_register_button.setFillColor (BLACK_COLOR);
  m_ok_button.setFillColor (BLACK_COLOR);
  m_name_label.setFillColor (BLACK_COLOR);
  m_password_label.setFillColor (BLACK_COLOR);
  m_name_display.setFillColor (BLACK_COLOR);
  m_password_display.setFillColor (BLACK_COLOR);
  m_message_display.setFillColor (BLACK_COLOR);
  m_new_dialog_sign.setFillColor (BLUE_COLOR);
  m_message_label.setFillColor (BLACK_COLOR);
  m_error_label.setFillColor (RED_COLOR);
  m_login_button.setPosition (CENTER_X - 150.f, CENTER_Y - 150.f);
  m_register_button.setPosition (CENTER_X + 50.f, CENTER_Y - 150.f);
  m_ok_button.setPosition (CENTER_X - 50.f, CENTER_Y + 150.f);
  m_name_label.setPosition (CENTER_X - 250.f, CENTER_Y - 50.f);
  m_password_label.setPosition (CENTER_X - 250.f, CENTER_Y + 50.f);
  m_error_label.setPosition (CENTER_X - 100.f, CENTER_Y + 200.f);
  m_name_display.setPosition (CENTER_X - 45.f, CENTER_Y - 45.f);
  m_password_display.setPosition (CENTER_X - 45.f, CENTER_Y + 55.f);
  m_message_display.setPosition (m_message_input_field.getPosition ().x + 10.f,
                                 m_message_input_field.getPosition ().y + 5.f);
  m_new_dialog_sign.setPosition (m_message_input_field.getPosition ().x - 30.f,
                                 m_message_input_field.getPosition ().y
                                     - m_message_input_field.getSize ().y / 2
                                     + 6);

  // Перерисовка ListModel
  redrawUsers ();

  // Вечный цикл
  loop ();
}

void GUI::loop(){

  // Пока окно открыто крутится цикл, в конце рисуем картинку
  for (; m_window.isOpen (); draw ())
    {

      // 1. Обработчик GUI событий
      while (m_window.pollEvent (m_event))
        {

          // Если окно закрывается перехватываем поток и его тоже закрываем,
          if (m_event.type == sf::Event::Closed)
            {
              m_inetgui_ptr->netstop ();
              m_net_thread_ptr->join ();
              m_window.close ();
            }

          if (m_event.type == sf::Event::MouseButtonPressed)
            mouseEventHandler ();
          if (m_event.type == sf::Event::TextEntered)
            inputTextEventHandler ();
          if (m_event.type == sf::Event::KeyPressed)
            keyEventHandler ();
        }

      // 2. Обработчик входящих от tcpclient пакетов
      netHandle ();
    }
}

void GUI::mouseEventHandler(){

  // Получаем координаты мыши во время нажатия
  sf::Vector2f mousePos
      = m_window.mapPixelToCoords (sf::Mouse::getPosition (m_window));

  // Если пользователь нажал на кнопку логина, то переходим в режим авторизации
  if (m_login_button.getGlobalBounds ().contains (mousePos))
    {
      m_is_entering_name = true;
      m_is_register_mode = false;
    }

  // Если пользователь нажал на кнопку register, то переходим в режим
  // регистрации
  if (m_register_button.getGlobalBounds ().contains (mousePos))
    {
      m_is_entering_name = true;
      m_is_register_mode = true; // Переходим в режим регистрации
    }

  // Пользователь нажимает ок и программа отправляет на сервер информацию
  if (m_ok_button.getGlobalBounds ().contains (mousePos))
    {

      // Если имя и или пароль пустые, то пользователь не залогинится
      if (m_name_input.getSize () == 0 || m_password_input.getSize () == 0)
        return;

      // Заправляем данные в пакет
      m_auth_pack.loginflag = !m_is_register_mode;
      memcpy (m_auth_pack.username, m_name_input.toAnsiString ().c_str (),
              NAME_LENGHT);
      memcpy (m_auth_pack.password, m_password_input.toAnsiString ().c_str (),
              PASSWORD_LENGHT);

      // Отправляем в интерфейс данные
      m_inetgui_ptr->setAuth (m_auth_pack);

      // Чуть-чуть ждем, чтобы tcpclient оклемался и отправил данные на сервер
      std::this_thread::sleep_for (std::chrono::seconds (1));

      // Попытка получить ответ из интерфейса (его должен туда запихнуть
      // tcpclient)
      if (m_inetgui_ptr->getAccept (m_accept_pack))
        {

          // Копируем имя, которое отправили серверу в пакет исходящего
          // сообщения, чтобы лишний раз этого не делать
          memcpy (m_output_msg.srcname, m_auth_pack.username, NAME_LENGHT);

          // Если сервер живой и пользователь правильно ввел данные, то
          // переходим в режим чата
          if (m_accept_pack.ans)
            {
              m_is_chat_mode = 1;
            }

          // Иначе говорим, что ошибка (сервер не принял наши данные, случается
          // когда при логине неверно имя или пароль введены)
          else
            {
              m_is_error_occured = 1;
            }
        }

      // Ответа от сервера нет, либо tcpclient запыхался, в любом случае ошибка
      else
        {
          m_is_error_occured = 1;
        }
    }

  // Кликаешь на имя - значит вводишь имя (возможно будет баг, если кликнуть
  // туда в режиме чата...)
  if (m_name_field.getGlobalBounds ().contains (mousePos))
    {
      m_is_entering_name = true;
    }

  // Если кликаешь на пароль, значит вводишь пароль
  if (m_password_field.getGlobalBounds ().contains (mousePos))
    {
      m_is_entering_name = false;
    }

  // Кликаешь на кнопку добавления (плюс слева от поля для ввода сообщений)
  if (m_new_dialog_sign.getGlobalBounds ().contains (mousePos))
    {
      m_is_adding_username = m_is_adding_username ? 0 : 1;
    }
}

void
GUI::inputTextEventHandler ()
{
  // Введенный символ сразу записываем
  m_entered_char = static_cast<char> (m_event.text.unicode);

  // Ввод имени или пароля в авторизации
  if (!m_is_chat_mode)
    {

      // Разрешаем только ASCII символы (0-127), исключая пробелы, и
      // обрабатываем Backspace
      // Исключаем пробел (32)
      if (m_entered_char >= 33 && m_entered_char <= 126)
        {

          // Записываем в поле имени
          if (m_is_entering_name && m_name_input.getSize () < 32)
            {
              m_name_input += m_entered_char;
            }

          // Записываем в поле пароля
          else if (m_password_input.getSize () < 32)
            {
              m_password_input += m_entered_char;
            }
        }

      // Backspace
      else if (m_event.text.unicode == 8)
        {

          // Удаляем символ в поле имени
          if (m_is_entering_name && !m_name_input.isEmpty ())
            {
              m_name_input.erase (m_name_input.getSize () - 1);
            }

          // Удаляем символ в поле пароля
          else if (!m_password_input.isEmpty ())
            {
              m_password_input.erase (m_password_input.getSize () - 1);
            }
        }

      // В любом случае обновляем поля ввода имени и пароля
      m_name_display.setString (m_name_input);
      m_password_display.setString (m_password_input);
    }

  // Если мы в чате
  else
    {
      // Разрешаем только ASCII символы (0-127), включая пробелы, и
      // обрабатываем Backspace
      // 99 символов максимум (+1 резерв для \0)

      // Записываем символ в текущее сообщение (оно также может быть вводом
      // имени нового пользователя)
      if ((m_entered_char >= 32 && m_entered_char <= 126)
          && m_current_message.getSize () < MSG_LENGHT - 1)
        {
          m_current_message += m_entered_char;
        }

      // Backspace
      else if (m_event.text.unicode == 8 && !m_current_message.isEmpty ())
        {
          m_current_message.erase (m_current_message.getSize () - 1);
        }

      // Все равно обновляем наше сообщение
      m_message_display.setString (m_current_message);
    }
}

void GUI::keyEventHandler(){

  // Кнопки пока функционируют только в чате, а в окне авторизации не
  // функционируют
  if (!m_is_chat_mode)
    return;
  // При нажатии стрелочки вверх выбираем пользователя выше по списку
  if (m_event.key.code == sf::Keyboard::Up)
    {
      m_selected_index = (m_selected_index - 1) % m_users.size ();
    }

  // При нажатии стрелочки вниз выбираем пользователя ниже по списку
  else if (m_event.key.code == sf::Keyboard::Down)
    {
      m_selected_index = (m_selected_index + 1) % m_users.size ();
    }

  // Enter
  else if (m_event.key.code == sf::Keyboard::Enter)
    {

      // Если сообщение пустое, то особо-то и делать ничего не надо
      if (m_current_message.isEmpty ())
        return;

      // Добавляем пользователя
      if (m_is_adding_username)
        {

          // Пробуем его найти среди уже имеющихся в клиенте (не на
          // сервере!!!), если не находим, то кидаем имя в конец вектора
          if (std::find (m_users.begin (), m_users.end (),
                         m_current_message.toAnsiString ())
              == m_users.end ())
            {
              m_users.push_back (m_current_message.toAnsiString ());
              m_chat_log.push_back ({ sf::Text () });
            }

          // Очищаем сообщение, обновляем строку, выключаем режим добавления
          // нового пользователя
          m_current_message.clear ();
          m_message_display.setString (m_current_message);
          m_is_adding_username = 0;

          // выходим, обязательно перерисовав ListModel пользователей
          return redrawUsers ();
        }

      // Обрабатываем сообщение
      // Копируем данные в пакет
      memcpy (m_output_msg.dstname, m_users[m_selected_index].c_str (),
              NAME_LENGHT);
      memcpy (m_output_msg.msg, m_current_message.toAnsiString ().c_str (),
              MSG_LENGHT);

      // Отправляем tcpclient данные
      m_inetgui_ptr->setOutputMsg (m_output_msg);

      // Настраиваем отображаемый текст
      m_message_label.setString (m_name_input + ": " + m_current_message);
      m_message_label.setPosition (
          20.f, 20.f + m_chat_log[m_selected_index].size () * 25.f);

      // Заносим текст в логи выбранного пользователя
      m_chat_log[m_selected_index].push_back (m_message_label);

      // Очищаем поле ввода, строку ввода
      m_current_message.clear ();
      m_message_display.setString (m_current_message);
    }
}

void
GUI::netHandle ()
{
  // Если мы не в чате, то выходим
  if (!m_is_chat_mode)
    return;

  // Если отсутствуют входящие сообщения или подтверждения аутентификации,
  // то пропускаем
  if (!m_inetgui_ptr->flags[FLAG_INPUT_MSG])
    return;

  // Получаем входящее сообщение
  if (m_inetgui_ptr->getInputMsg (this->m_input_msg_pack))
    {
      // Ищем пользователя среди добавленных нами пользователей
      auto it = std::find (m_users.begin (), m_users.end (),
                           std::string (m_input_msg_pack.srcname));
      if (it != m_users.end ())
        {
          // Нашли, добавляем сообщение к этому пользователю
          sf::Text msg (sf::String (m_input_msg_pack.srcname) + ": "
                            + sf::String (m_input_msg_pack.msg),
                        m_font, 14);
          msg.setFillColor (BLACK_COLOR);
          msg.setPosition (20.f,
                           20.f
                               + m_chat_log[m_selected_index].size ()
                                     * 25.f); // Располагаем сообщения
                                              // друг под другом
          m_chat_log[it - m_users.begin ()].push_back (msg);
          redrawUsers ();
        }
      else
        {
          // Не нашли, добавляем пользователя + к нему сообщение
          m_users.push_back (m_input_msg_pack.srcname);
          sf::Text msg (sf::String (m_input_msg_pack.srcname) + ": "
                            + sf::String (m_input_msg_pack.msg),
                        m_font, 14);
          msg.setFillColor (BLACK_COLOR);
          msg.setPosition (20.f,
                           20.f); // Располагаем сообщения друг под другом
          m_chat_log.push_back ({ msg });
          redrawUsers ();
        }
    }
}

void
GUI::draw ()
{

  // Очистка
  m_window.clear (GRAY_COLOR);

  // Режим аутентификация
  if (!m_is_chat_mode)
    {

      // Login
      if (m_is_register_mode)
        {
          m_register_button.setFillColor (BLUE_COLOR);
          m_login_button.setFillColor (BLACK_COLOR);
        }

      // Register
      else
        {
          m_login_button.setFillColor (BLUE_COLOR);
          m_register_button.setFillColor (BLACK_COLOR);
        }

      // Рисуем gui элементы окна авторизации
      m_window.draw (m_login_button);
      m_window.draw (m_register_button);
      m_window.draw (m_name_field);
      m_window.draw (m_password_field);
      m_window.draw (m_name_label);
      m_window.draw (m_password_label);
      m_window.draw (m_name_display);
      m_window.draw (m_password_display);
      m_window.draw (m_ok_button);

      // Если есть ошибка рисуем еще сообщение об ошибке
      if (m_is_error_occured)
        {
          m_window.draw (m_error_label);
        }
    }

  // Режим чата
  else
    {

      // Выбираем цвет для кнопки добавления нового пользователя
      m_new_dialog_sign.setFillColor (m_is_adding_username ? GREEN_COLOR
                                                           : BLUE_COLOR);
      // Рисуем ListModel
      for (const auto &text : m_user_name_text)
        {
          m_window.draw (text);
        }

      // Отображаем лог сообщений выбранного пользователя
      for (const auto &line : m_chat_log[m_selected_index % m_users.size ()])
        {
          m_window.draw (line);
        }

      // Перекрашиваем ListModel
      for (size_t i = 0; i < m_user_name_text.size (); ++i)
        {

          // Выбранный пользователь
          if (i == m_selected_index)
            {
              m_user_name_text[i].setFillColor (BLUE_COLOR);
            }

          // Невыбранный пользователь
          else
            {
              m_user_name_text[i].setFillColor (BLACK_COLOR);
            }
        }

      // Рисуем все GUI элементы окна чата
      m_window.draw (m_message_input_field);
      m_window.draw (m_message_display);
      m_window.draw (m_new_dialog_sign);
    }

  // Показываем
  m_window.display ();
}

void GUI::redrawUsers(){

  // Очищаем текущий вектор пользователей
  m_user_name_text.clear ();

  // Запасаемся памятью на всех
  m_user_name_text.reserve (m_users.size ());

  // Перерисовываем положение каждого пользователя
  for (size_t i = 0; i < m_users.size (); ++i)
    {
      m_user_name_text.push_back (sf::Text (m_users[i], m_font, 18));
      m_user_name_text[i].setFont (m_font);
      m_user_name_text[i].setString (m_users[i]);
      m_user_name_text[i].setPosition (600.f, 20.f + i * 20.f);
    }
}

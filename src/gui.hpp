#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

namespace GUI
{
   class Gui
   {

   public:
	   Gui(const sf::String& _name, uint32_t _x, uint32_t _y);
      ~Gui(){};

      void draw(sf::RenderWindow& _window);
      void process();
      void activate(sf::Event::KeyEvent *sf_key_event);
      // virtual void activate(sf::Event::MouseButtonEvent *_sf_mouse_button_event){};
      // virtual void activate(sf::Event::TextEvent *_text_event){};
      // virtual void activate(sf::Event::JoystickButtonEvent *_joy_button_event){};
      // virtual unsigned int get_state(){return state;};
      // virtual void set_state(unsigned int _state){state=_state;};

      void unfocus(){m_cback = sf::Color::White;};
      void focus(){m_cfront = m_cback = sf::Color::Yellow;};
      void set_text(const sf::String& _name){m_text.setString(_name);};

   private:
      sf::Color m_cfront, m_cback;
      sf::Text m_text;
      sf::Font m_font;
   };
}
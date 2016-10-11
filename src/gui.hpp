#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <functional>

namespace GUI
{
   class Gui
   {

   public:
	   Gui(const sf::String& _name, uint32_t _x, uint32_t _y, 
		   std::function<void()> _onClick = [](){});
      ~Gui(){};

      void draw(sf::RenderWindow& _window);
      void process();
      void activate(sf::Event::KeyEvent *sf_key_event);
      // virtual void activate(sf::Event::MouseButtonEvent *_sf_mouse_button_event){};
      // virtual void activate(sf::Event::TextEvent *_text_event){};
      // virtual void activate(sf::Event::JoystickButtonEvent *_joy_button_event){};
      // virtual unsigned int get_state(){return state;};
      // virtual void set_state(unsigned int _state){state=_state;};
	  void click() { if (m_onClick) m_onClick(); }

      void unfocus(){m_cback = sf::Color::White;};
      void focus(){m_cfront = m_cback = sf::Color::Yellow;};
      void setText(const sf::String& _name){m_text.setString(_name);};
	  void setPosition(sf::Vector2f _pos) { m_text.setPosition(_pos); }
   protected:
      sf::Color m_cfront, m_cback;
      sf::Text m_text;
      sf::Font m_font;

	  std::function<void()> m_onClick;
   };

   //element with two labels
   class ExtGui : public Gui
   {
   public:
	   ExtGui(const sf::String& _name, uint32_t _x, uint32_t _y,
		   std::function<void()> _onClick = [](){}, sf::Vector2f _vec = sf::Vector2f(0.f,0.f));

	   void draw(sf::RenderWindow& _window);
	   //relative to the first label
	   void setPosition2(sf::Vector2f _pos) { m_text2.setPosition(m_text.getPosition() + _pos); };
	   void setText2(const sf::String& _text) { m_text2.setString(_text); }
   private:
	   sf::Text m_text2;
   };
}
#include "gui.hpp"
#include "resourcemanager.hpp"
#include <cmath>

namespace GUI
{
	// ******************************************************** //

	GuiElement::GuiElement(const sf::String& _name, uint32_t _x, uint32_t _y, std::function<void()> _onClick)
		:m_onClick(_onClick)
	{
		m_font = *g_resourceManager->getFont("suburbia");

		m_cfront = sf::Color::Black;
		m_cback = sf::Color::White;

		m_text.setFont(m_font);
		m_text.setCharacterSize(75);
		m_text.setString(_name);
		m_text.setPosition(sf::Vector2f(static_cast<float>(_x), static_cast<float>(_y)));
		m_text.setScale(sf::Vector2f(1.f, 1.f));
	}

	void GuiElement::draw(sf::RenderWindow& _window)
	{
		m_text.setFillColor(m_cfront);
		_window.draw(m_text);

	}

	void GuiElement::process()
	{
		m_cfront.r += static_cast<sf::Uint8>(std::ceil(0.1f*((float) (m_cback.r-m_cfront.r))));
		m_cfront.g += static_cast<sf::Uint8>(std::ceil(0.1f*((float) (m_cback.g-m_cfront.g))));
		m_cfront.b += static_cast<sf::Uint8>(std::ceil(0.1f*((float) (m_cback.b-m_cfront.b))));
	}

	void GuiElement::activate(sf::Event::KeyEvent *sf_key_event)
	{

	}

	// ********************************************** //

	ExtGuiElement::ExtGuiElement(const sf::String& _name, uint32_t _x, uint32_t _y,
		std::function<void()> _onClick, sf::Vector2f _position)
		: GuiElement(_name, _x, _y, _onClick)
	{
		m_text2.setFont(m_font);
		m_text2.setCharacterSize(75);
		setPosition2(_position);
	}

	void ExtGuiElement::draw(sf::RenderWindow& _window)
	{
		GuiElement::draw(_window);
		_window.draw(m_text2);
	}

	// ********************************************** //

	Gui::Gui()
		: m_controllerMove(false),
		m_currentElement(end())
	{}

	void Gui::init()
	{
		m_currentElement = begin();
		(*m_currentElement)->focus();

		m_buttonSound.setBuffer(*g_resourceManager->getSound("sound_item"));
	}

	void Gui::draw(sf::RenderWindow& _window)
	{
		for (auto& el : *this) el->draw(_window);
	}

	void Gui::process()
	{
		float axis = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		if (m_controllerMove)
		{
			if(abs(axis) < 40.f) m_controllerMove = false;
		}
		else
		{
			if (axis > 70.f) { next(); m_controllerMove = true; }
			else if (axis < -70.f) { prev(); m_controllerMove = true; }
		}
		for (auto& el : *this) el->process();
	}

	void Gui::processEvents(sf::Event& _event)
	{
		switch (_event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (_event.key.code == sf::Keyboard::Up)
			{
				prev();
			}
			else if (_event.key.code == sf::Keyboard::Down)
			{
				next();
			}
			else if (_event.key.code == sf::Keyboard::Enter)
			{
				(*m_currentElement)->click();
				m_buttonSound.play();
			}
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			break;
		}
		case sf::Event::JoystickButtonPressed:
		{
			if (_event.joystickButton.joystickId == 0 && _event.joystickButton.button == 0)
			{
				(*m_currentElement)->click();
				m_buttonSound.play();
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}

	void Gui::prev()
	{
		if (m_currentElement != begin())
		{
			(*m_currentElement--)->unfocus();
			(*m_currentElement)->focus();
		}
		m_buttonSound.play();
	}

	void Gui::next()
	{
		(*m_currentElement++)->unfocus();
		if (m_currentElement == end()) m_currentElement--;
		(*m_currentElement)->focus();

		m_buttonSound.play();
	}
}
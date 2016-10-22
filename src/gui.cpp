#include "gui.hpp"
#include "resourcemanager.hpp"

namespace GUI
{


	// ******************************************************** //

	GuiElement::GuiElement(const sf::String& _name, uint32_t _x, uint32_t _y, std::function<void()> _onClick)
		:m_onClick(_onClick)
    {
        m_font = *g_resourceManager.getFont("suburbia");

        m_cfront = sf::Color::Black;
        m_cback = sf::Color::White;

        m_text.setFont(m_font);
        m_text.setCharacterSize(75);
        m_text.setString(_name);
        m_text.setPosition(_x, _y);
        m_text.setScale(1., 1.);
    }

    void GuiElement::draw(sf::RenderWindow& _window)
    {
        m_text.setColor(m_cfront);
        _window.draw(m_text);

    }

    void GuiElement::process()
    {
        m_cfront.r += ceil(0.1*((float) (m_cback.r-m_cfront.r)));
        m_cfront.g += ceil(0.1*((float) (m_cback.g-m_cfront.g)));
        m_cfront.b += ceil(0.1*((float) (m_cback.b-m_cfront.b)));
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

	void Gui::init()
	{
		m_currentElement = begin();
		(*m_currentElement)->focus();

		m_buttonSound.setBuffer(*g_resourceManager.getSound("sound_item"));
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
			else if (_event.key.code == sf::Keyboard::Return)
			{
				(*m_currentElement)->click();
				m_buttonSound.play();
			}
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
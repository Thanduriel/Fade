#include "gui.hpp"
#include "resourcemanager.hpp"

namespace GUI
{

	Gui::Gui(const sf::String& _name, uint32_t _x, uint32_t _y)
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

    void Gui::draw(sf::RenderWindow& _window)
    {
        m_text.setColor(m_cfront);
        _window.draw(m_text);

    }

    void Gui::process()
    {
        m_cfront.r += ceil(0.1*((float) (m_cback.r-m_cfront.r)));
        m_cfront.g += ceil(0.1*((float) (m_cback.g-m_cfront.g)));
        m_cfront.b += ceil(0.1*((float) (m_cback.b-m_cfront.b)));
    }

    void Gui::activate(sf::Event::KeyEvent *sf_key_event)
    {

    }
}
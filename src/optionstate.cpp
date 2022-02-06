#include <iostream>
#include <array>

#include "optionstate.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"
#include "camera.hpp"

namespace Graphic{
	sf::View g_camera;
}


//possible choices for the corresponding settings
const std::array<std::string, 7> DESCRIPTIONS =
{
	"Places a mine at the players location.\nAfter a short delay any collision causes the mine to explode.", 
	"Places a sentinel at the players location.\nAfter a short delay the sentinel lights up for some time.",
	"Temporarily the user shoots 3 projectiles instead of one.",
	"Temporarily the user light radius is vastly higher.",
	"Regenerates health back to full.",
	"Temporarily increases the users movement speed.",
	"The user temporarily reflects projectiles that would normally hit him."
};


using namespace sf;

namespace State{
	OptionState::OptionState(sf::RenderWindow& _window) :
		m_window(_window)
    {
        font = *g_resourceManager->getFont("suburbia");

        uint32_t left = Constants::g_windowSizeX / 2 - 450;
		Vector2f rightOff = Vector2f(600.f, 0.f);

        title.setFont(font);
        title.setCharacterSize(200);
        title.setString("FADE");
        title.setPosition(sf::Vector2f(left + 150.f,50.f));
        title.setScale(sf::Vector2f(1.f, 1.f));
        title.setFillColor(sf::Color::White);
		


		m_items.emplace_back(new Game::Mine(sf::Vector2f(200.f, 300.f)));
		m_items.emplace_back(new Game::Sentinel(sf::Vector2f(200.f, 400.f)));
		m_items.emplace_back(new Game::ClusterGun(sf::Vector2f(200.f, 500.f)));
		m_items.emplace_back(new Game::LightAura(sf::Vector2f(200.f, 600.f)));
		m_items.emplace_back(new Game::HealthBoost(sf::Vector2f(200.f, 700.f)));
		m_items.emplace_back(new Game::SpeedBoost(sf::Vector2f(200.f, 800.f)));
		m_items.emplace_back(new Game::Shield(sf::Vector2f(200.f, 900.f)));

		m_itemTexts.resize(m_items.size());
		for (size_t i = 0; i < m_items.size(); ++i)
		{
			m_itemTexts[i].setString(DESCRIPTIONS[i]);
			m_itemTexts[i].setCharacterSize(40);
			m_itemTexts[i].setPosition(m_items[i]->position() + Vector2f(50.f, -56.f * 0.5f));
			m_itemTexts[i].setScale(sf::Vector2f(1.f, 1.f));
			m_itemTexts[i].setFillColor(sf::Color::White);
			m_itemTexts[i].setFont(font);
		}
    }

    void OptionState::process()
    {
        for (auto& gui : m_gui)
            gui->process();
    }

    void OptionState::processEvents(sf::Event& _event)
    {
		GameState::processEvents(_event);
		m_gui.processEvents(_event);
    }

    void OptionState::draw(sf::RenderWindow& _window)
    {
        _window.draw(title);
        for (auto& gui : m_gui)
            gui->draw(_window);

		for (auto& itm : m_items) itm->draw(_window);

		for (auto& text : m_itemTexts) _window.draw(text);
    }

	// *********************************************** //

	void OptionState::refreshGuiElement()
	{
	}
}
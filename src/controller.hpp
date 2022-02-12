#pragma once

#include "pawn.hpp"
#include "input/inputmanager.hpp"
#include <array>

namespace Game{

	class Controller
	{
	public:
		Controller() : m_pawn(nullptr), m_id(-1), m_playerColor(PlayerColor::White) {}
		virtual ~Controller() = default;

		virtual void process(){}
		virtual void processEvent(sf::Event&) {}

		void possess(Pawn* _pawn) { m_pawn = _pawn; }
		bool hasPawn() const { return m_pawn != nullptr; }
		Pawn* getPawn() const { return m_pawn; }
		uint32_t getId() const { return m_id; }

		void setPlayerColor(PlayerColor _color) { m_playerColor = _color; }
		PlayerColor getPlayerColor() const { return m_playerColor; }
	protected:
		Pawn* m_pawn;
		uint32_t m_id;

		PlayerColor m_playerColor;
	};

	class PlayerController : public Controller
	{
	public:
		PlayerController(int _id, std::unique_ptr<Input::InputInterface>& _interface) 
			: m_inputs(std::move(_interface)), m_fireCd(0)
		{ 
			m_id = _id; 
		}

		void process() override;
		void processEvent(sf::Event& _event) override;

	private:
		std::unique_ptr<Input::InputInterface> m_inputs;
		int m_fireCd;
	};

	class AiController : public Controller
	{
	public:
		void process() override;
	};
}
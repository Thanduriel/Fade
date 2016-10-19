#pragma once

#include "pawn.hpp"
#include <array>

namespace Game{

	class Controller
	{
	public:
		Controller() : m_playerColor(PlayerColor::White) {}

		virtual void process(){};
		virtual void processEvent(sf::Event& _event){};

		void possess(Pawn* _pawn) { m_pawn = _pawn; }
		bool hasPawn() { return m_pawn != 0; }
		Pawn* getPawn() { return m_pawn; }
		uint32_t getId() { return m_id; }

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
		PlayerController(int _id) : m_fireCd(0){ m_id = _id; }

		void process() override;
		void processEvent(sf::Event& _event) override;

	private:
		int m_fireCd;
	};

	class AiController : public Controller
	{
	public:
		void process() override;
	};
}
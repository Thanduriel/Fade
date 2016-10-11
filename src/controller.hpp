#pragma once

#include "pawn.hpp"

namespace Game{
	class Controller
	{
	public:
		virtual void process(){};
		virtual void processEvent(sf::Event& _event){};

		void possess(Pawn* _pawn) { m_pawn = _pawn; }
		bool hasPawn() { return m_pawn != 0; }
		Pawn* getPawn() { return m_pawn; }
		uint32_t getId() { return m_id; }
	protected:
		Pawn* m_pawn;
		uint32_t m_id;
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
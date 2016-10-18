#pragma once

#include "pawn.hpp"
#include <array>

namespace Game{

	class Controller
	{
	public:
		Controller() : m_colorId(0) {}

		virtual void process(){};
		virtual void processEvent(sf::Event& _event){};

		void possess(Pawn* _pawn) { m_pawn = _pawn; }
		bool hasPawn() { return m_pawn != 0; }
		Pawn* getPawn() { return m_pawn; }
		uint32_t getId() { return m_id; }

		void setColorId(size_t _color) { m_colorId = _color; }
		size_t getColorId() const { return m_colorId; }
	protected:
		Pawn* m_pawn;
		uint32_t m_id;

		size_t m_colorId;
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
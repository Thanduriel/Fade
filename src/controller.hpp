#pragma once

#include "pawn.hpp"

namespace Game{
	class Controller
	{
	public:
		virtual void process(){};
		virtual void processEvent(sf::Event& _event){};

		void possess(Pawn* _pawn) { m_pawn = _pawn; }
	protected:
		Pawn* m_pawn;
	};

	class PlayerController : public Controller
	{
	public:
		PlayerController(int _id) :m_id(_id){}

		void process() override;
		void processEvent(sf::Event& _event) override;
	private:
		uint m_id;
	};
}
#pragma once

#include "pawn.hpp"

namespace Game{
	class Controller
	{
	public:
		virtual void process(){};

		void possess(Pawn* _pawn) { m_pawn = _pawn; }
	protected:
		Pawn* m_pawn;
	};

	class PlayerController : public Controller
	{
	public:
		PlayerController(int _id) :m_id(_id){}

		void process() override;
	private:
		int m_id;
	};
}
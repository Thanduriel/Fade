#pragma once

#include "pawn.hpp"

namespace Game{
	class Controller
	{
	public:
		void process();

		void possess(Pawn* _pawn) { m_pawn = _pawn; }
	protected:
		Pawn* m_pawn;
	};
}
#pragma once
#include <vector>
#include <array>

namespace Stats{

	enum Stat{
		Deaths,
		Kills,
		Count
		//todo: powerups
	};

	typedef public std::array<int, Stat::Count> PlayerStat;

	class StatManager
	{
	public:
		int Get(int _id, Stat _stat) { return m_playerStats.size() > _id ? m_playerStats[_id][_stat] : 0; };
		void Add(int _id, Stat _stat) { if (_id < 0) return; if (_id >= m_playerStats.size()) m_playerStats.resize(_id+1); ++m_playerStats[_id][_stat]; }

		void Reset() { m_playerStats.clear(); }
	private:
		std::vector<PlayerStat> m_playerStats;
	};

	extern StatManager g_statManager; //defined in main.cpp

}
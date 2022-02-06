#pragma once
#include <vector>
#include <array>
#include <algorithm>

namespace Stats{

	enum Stat{
		Deaths,
		Kills,
		Id, //the id is set before sorting
		ShotsFired,
		ShotsHit,
		ItemsUsed,
		IsActive, // is this id in use
		Count
		//todo: powerups
	};

	typedef public std::array<int, Stat::Count> PlayerStat;

	class StatManager
	{
	public:
		StatManager() : m_isSorted(false){}

		int Get(int _id, Stat _stat) { return m_playerStats.size() > _id ? m_playerStats[_id][_stat] : 0; };
		void Add(int _id, Stat _stat) { if (_id < 0) return; if (_id >= m_playerStats.size()) m_playerStats.resize(_id+1); ++m_playerStats[_id][_stat]; }

		int getMax(Stat _stat) { int max = 0; for (auto& st : m_playerStats) if (st[_stat] > max) max = st[_stat]; return max; }

		int getPlayerCount(){ return static_cast<int>(m_playerStats.size()); }
		void sort(Stat _pred) {
			for (size_t i = 0; i < m_playerStats.size(); ++i) 
				m_playerStats[i][Stat::Id] = static_cast<int>(i);
			std::sort(m_playerStats.begin(), m_playerStats.end(),
				[=](const PlayerStat& _first, const PlayerStat& _second){return _first[_pred] < _second[_pred]; });
		}
		void Reset() { m_playerStats.clear(); }
	private:
		std::vector<PlayerStat> m_playerStats;

		bool m_isSorted;
	};

	extern StatManager g_statManager; //defined in main.cpp

}
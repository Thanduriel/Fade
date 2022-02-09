#include "stats.hpp"

namespace Stats {

	int StatManager::get(int _id, Stat _stat) const
	{
		const size_t id = static_cast<size_t>(_id);
		return m_playerStats.size() > id ? m_playerStats[id][static_cast<size_t>(_stat)] : 0;
	}

	void StatManager::add(int _id, Stat _stat)
	{
		if (_id < 0) return; 
		
		const size_t id = static_cast<size_t>(_id);
		if (id >= m_playerStats.size())
			m_playerStats.resize(id + 1);
		
		++m_playerStats[id][static_cast<size_t>(_stat)];
	}

	int StatManager::getMax(Stat _stat) const
	{ 
		int max = 0; 
		for (auto& st : m_playerStats) 
			if (st[static_cast<size_t>(_stat)] > max) max = st[static_cast<size_t>(_stat)];
		return max; 
	}

	void StatManager::sort(Stat _pred) 
	{
		for (size_t i = 0; i < m_playerStats.size(); ++i)
			m_playerStats[i][static_cast<size_t>(Stat::Id)] = static_cast<int>(i);
		std::sort(m_playerStats.begin(), m_playerStats.end(),
			[=](const PlayerStat& _first, const PlayerStat& _second) 
			{
				return _first[static_cast<size_t>(_pred)] < _second[static_cast<size_t>(_pred)];
			});
	}
}
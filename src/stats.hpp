#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

namespace Stats{

	enum struct Stat {
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

	using PlayerStat = std::array<int, static_cast<size_t>(Stat::Count)>;

	class StatManager
	{
	public:
		StatManager() : m_isSorted(false){}

		int get(int _id, Stat _stat) const;
		void add(int _id, Stat _stat);

		int getMax(Stat _stat) const;

		int getPlayerCount() const { return static_cast<int>(m_playerStats.size()); }
		
		void sort(Stat _pred);

		void reset() { m_playerStats.clear(); }
	private:
		std::vector<PlayerStat> m_playerStats;

		bool m_isSorted;
	};

	extern std::unique_ptr<StatManager> g_statManager; //defined in main.cpp

}
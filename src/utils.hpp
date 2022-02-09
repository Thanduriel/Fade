#include <stdint.h>
#include <random>

namespace util{
	inline unsigned int rnd()
	{
		static uint32_t num = std::random_device()();
		num ^= num << 13;
		num ^= num >> 17;
		num ^= num << 5;

		return num;
	}

	inline uint32_t rand(uint32_t _max, uint32_t _min = 0)
	{
		return(rnd() % (_max + 1 - _min)) + _min;
	}
}
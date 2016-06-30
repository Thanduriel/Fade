#include <stdint.h>

namespace util{
	inline unsigned int rnd()
	{
		static unsigned int num = rand();
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
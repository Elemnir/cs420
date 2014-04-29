#ifndef ND_RAND_H_INCLUDED
#define ND_RAND_H_INCLUDED

std::uint_fast32_t nd_rand();

#ifdef __MINGW32__
	#include <windows.h>
	#include <Ntsecapi.h>
	inline std::uint_fast32_t nd_rand()
	{
		std::uint_fast32_t rv;
		RtlGenRandom(&rv, sizeof(rv));
		return rv;
	}
#else
	#include <random>
	inline std::uint_fast32_t nd_rand()
	{
		return std::random_device{}();
	}
#endif // __MINGW32__

#endif // ND_RAND_H_INCLUDED

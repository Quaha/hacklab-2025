#pragma once

#include <cstdint>
#include <vector>

struct Generator {

	using result_type = uint32_t;

	static uint64_t a;
	static uint64_t mod;

	uint64_t state;

	uint32_t degs[32];

	Generator();
	Generator(uint64_t seed);
	Generator(uint64_t seed, int shift);

	result_type operator()();

	static constexpr result_type min() { return 0u; }
	static constexpr result_type max() { return 2147483647ull - 1ull; }

	void move(int shift);
};
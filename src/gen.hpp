#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

struct Generator {

	using result_type = uint32_t;

	static uint64_t a;
	static uint64_t mod;

	uint64_t state;

	uint64_t degs[32];

	Generator();
	Generator(uint64_t seed);
	Generator(uint64_t seed, int shift);

	result_type operator()();

	static constexpr result_type min() { return 0u; }
	static constexpr result_type max() { return 2147483647ull - 1ull; }

	static constexpr float fmin() { return 0.0f; }
	static constexpr float fmax() { return 2147483646.0f; }

	static constexpr float flen() { return 2147483646.0f; }

	void move(int shift);
};

#define VGENSZ 64

struct VGenerator {

	using result_type = uint32_t;

	static uint64_t a;
	static uint64_t mod;

	uint64_t state[VGENSZ];

	uint64_t degs[VGENSZ];


	uint64_t shift_par;


	VGenerator(uint64_t seed, int shift);

	void copy(int ci, int otheri, uint32_t* data);
	void change();

	static constexpr result_type min() { return 0u; }
	static constexpr result_type max() { return 2147483647ull - 1ull; }

	static constexpr float fmin() { return 0.0f; }
	static constexpr float fmax() { return 2147483646.0f; }

	static constexpr float flen() { return 2147483646.0f; }
};

inline float uni_f_dist01(Generator& gen) {
	return (static_cast<float>(gen()) - Generator::fmin()) / Generator::flen();
}

inline float uni_f_distab(Generator& gen, float a, float b) {
	return a + (b - a) * uni_f_dist01(gen);
}

inline float exp_f_dist(Generator& gen, float lambda) {
	return -std::log(1.0f - uni_f_dist01(gen)) / lambda;
}

inline float bern_f_dist(Generator& gen, float p) {
	return uni_f_dist01(gen) <= p;
}
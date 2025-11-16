#include "gen.hpp"

uint64_t Generator::a = 16807ull;
uint64_t Generator::mod = 2147483647ull;

void initDegs(uint32_t degs[32], uint64_t a, uint64_t mod) {

	uint64_t value = 1ull;
	for (int i = 0; i < 32; i++) {
		degs[i] = value;
		value *= a;
		value %= mod;
	}
}

Generator::Generator() {
	state = 1ull;

	initDegs(degs, a, mod);
}

Generator::Generator(uint64_t seed) {
	state = seed % mod;

	initDegs(degs, a, mod);
}

Generator::Generator(uint64_t seed, int shift) {
	state = seed % mod;

	uint64_t value = a;
	for (int i = 0; i < 32; i++) {
		if (shift & (1 << i)) {
			state *= value;
			state %= mod;
		}
		value *= value;
		value %= mod;
	}
}

uint32_t Generator::operator()() {
	state *= a;
	state %= mod;
	return state;
}

void Generator::move(int shift) {
	uint64_t shift_value = 1ull;

	for (int i = 0; i < 32; i++) {
		if (shift & (1 << i)) {
			shift_value *= degs[i];
			shift_value %= mod;
		}
	}

	state *= shift_value;
	state %= mod;
}

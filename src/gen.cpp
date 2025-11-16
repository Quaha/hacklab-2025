#include "gen.hpp"

uint64_t Generator::a = 16807ull;
uint64_t Generator::mod = 2147483647ull;

void initDegs(uint64_t degs[32], uint64_t a, uint64_t mod) {

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

VGenerator::VGenerator(uint64_t seed, int shift) {
	uint32_t cstate = seed % mod;

	uint64_t value = a;
	for (int i = 0; i < 32; i++) {
		if (shift & (1 << i)) {
			cstate *= value;
			cstate %= mod;
		}
		value *= value;
		value %= mod;
	}

	uint64_t v = 1ull;

	for (int i = 0; i < VGENSZ; i++) {
		degs[i] = v;
		v *= a;
		v %= mod;
	}

	shift_par = v;

	for (int i = 0; i < VGENSZ; i++) {
		state[i] = (cstate * degs[i]) % mod;
	}


}

void VGenerator::copy(int ci, int otheri, uint32_t* data) {
	for (int i = 0; i < ci; i++) {
		data[otheri + i] = state[ci];
	}
}

void VGenerator::change() {
	for (int i = 0; i < VGENSZ; ++i) {
		state[i] *= shift_par;
		state[i] %= mod;
	}
}
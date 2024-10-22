#include "Clock.hpp"
#include <chrono>

mk::Clock::Clock(): last_reset(std::chrono::steady_clock::now()) {}

float mk::Clock::restart() {
	auto                         now  = std::chrono::steady_clock::now();
	std::chrono::duration<float> diff = now - last_reset;
	last_reset                        = now;
	return diff.count();
}

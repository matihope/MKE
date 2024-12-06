#pragma once
#include <chrono>

namespace mk {
	class Clock {
	public:
		Clock();
		float restart();

		float getElapsedTime();

	private:
		// Steady clock, because as cppreference says:
		// "[This clock] is most suitable for measuring interval."
		std::chrono::time_point<std::chrono::steady_clock> last_reset;
	};
}

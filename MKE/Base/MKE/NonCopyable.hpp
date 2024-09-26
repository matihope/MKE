#pragma once

namespace mk {
	class NonCopyable {
	public:
		NonCopyable()          = default;
		virtual ~NonCopyable() = 0;

		NonCopyable(const NonCopyable&)            = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
    inline NonCopyable::~NonCopyable() {}
}

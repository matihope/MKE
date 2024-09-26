#pragma once

#include "MKE/StrUtils.hpp"
#include <stdexcept>

namespace mk::exceptions {
	class MkException: public std::runtime_error {
	public:
		template<class... Args>
		MkException(Args&&... args): std::runtime_error(strConcat(std::forward<Args>(args)...)){};
	};
}

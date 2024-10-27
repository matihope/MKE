#include "WorldEntity.hpp"

namespace mk {
	EntityID detail::id_counter() {
		static EntityID newest_id{};
		return newest_id++;
	}
}  // namespace mk

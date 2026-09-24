#pragma once

// Force-included (via `-include`) into each example's translation unit by the
// add_example_assets() CMake macro. It is NOT compiled into the engine
// libraries, so CUSTOM_ASSETS_PATH is only ever defined here, in the example
// TU. The static initializer runs before main() and hands the example's asset
// directory to the engine as a runtime value, which every ResPath then
// resolves against — no matter which library the load happens in.
#ifdef CUSTOM_ASSETS_PATH

#include "MKE/ResPath.hpp"

namespace {
	const bool mke_auto_asset_root = (mk::setAssetRoot(CUSTOM_ASSETS_PATH), true);
}  // namespace

#endif

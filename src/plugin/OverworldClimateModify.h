#pragma once

#include "ll/api/plugin/NativePlugin.h"

namespace overworld_climate_modify {

class OverworldClimateModify {

public:
    static OverworldClimateModify& getInstance();

    OverworldClimateModify(ll::plugin::NativePlugin& self) : mSelf(self) {}

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    /// @return True if the plugin is loaded successfully.
    bool load();

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the plugin.
    // /// @return True if the plugin is unloaded successfully.
    // bool unload();

    std::filesystem::path getConfigDirPath();
    std::filesystem::path getConfigFilePath();

private:
    ll::plugin::NativePlugin& mSelf;
};

} // namespace overworld_climate_modify

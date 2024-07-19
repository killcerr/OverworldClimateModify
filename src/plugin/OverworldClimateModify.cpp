#include "plugin/OverworldClimateModify.h"

#include <memory>

#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/RegisterHelper.h"

namespace overworld_climate_modify {

static std::unique_ptr<OverworldClimateModify> instance;

OverworldClimateModify& OverworldClimateModify::getInstance() { return *instance; }

bool OverworldClimateModify::load() {
    getSelf().getLogger().info("Loading...");
    // Code for loading the plugin goes here.
    return true;
}

bool OverworldClimateModify::enable() {
    getSelf().getLogger().info("Enabling...");
    // Code for enabling the plugin goes here.
    return true;
}

bool OverworldClimateModify::disable() {
    getSelf().getLogger().info("Disabling...");
    // Code for disabling the plugin goes here.
    return true;
}

std::filesystem::path OverworldClimateModify::getConfigDirPath() { return getSelf().getConfigDir(); }

std::filesystem::path OverworldClimateModify::getConfigFilePath() { return getConfigDirPath() / "config.json"; }
} // namespace overworld_climate_modify

LL_REGISTER_PLUGIN(overworld_climate_modify::OverworldClimateModify, overworld_climate_modify::instance);

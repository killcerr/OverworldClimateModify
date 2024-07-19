#include "ClimateModifyConfig.hpp"
#include "OverworldClimateModify.h"

#include "ll/api/Config.h"

climate_modify_config::Config climate_modify_config::Config::instance() {
    static climate_modify_config::Config config;
    static bool                          loaded = false;
    if (loaded) return config;
    if (!std::filesystem::exists(overworld_climate_modify::OverworldClimateModify::getInstance().getConfigFilePath())) {
        std::filesystem::create_directories(
            overworld_climate_modify::OverworldClimateModify::getInstance().getConfigDirPath()
        );
        ll::config::saveConfig(
            config,
            overworld_climate_modify::OverworldClimateModify::getInstance().getConfigFilePath()
        );
    } else {
        ll::config::loadConfig(
            config,
            overworld_climate_modify::OverworldClimateModify::getInstance().getConfigFilePath()
        );
    }
    loaded = true;
    return config;
}
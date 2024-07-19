#include "ClimateModifyConfig.hpp"
#include "Expr.hpp"
#include "OverworldClimateModify.h"


#include "ll/api/memory/Hook.h"
#include "mc/world/level/biome/TerrainShaper.h"


LL_AUTO_STATIC_HOOK(
    TerrainShaper_factor,
    HookPriority::Normal,
    "?factor@TerrainShaper@@QEBAMMMM@Z",
    float,
    TerrainShaper* self,
    float          continentalness,
    float          erosion,
    float          weirdness
) {
    auto                                                ori = origin(self, continentalness, erosion, weirdness);
    std::unordered_map<std::string, std::vector<float>> params{
        {"ori",             {ori}            },
        {"continentalness", {continentalness}},
        {"erosion",         {erosion}        },
        {"weirdness",       {weirdness}      },
    };
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>>
        functions{
            {"info", [](const std::vector<std::vector<float>>& args) {
                 for (auto i : args[0]) {
                     overworld_climate_modify::OverworldClimateModify::getInstance().getSelf().getLogger().info(
                         "{}:{}",
                         __FUNCSIG__,
                         i
                     );
                 }
                 return args[0];
             }}
    };
    auto res = expr::eval(climate_modify_config::Config::instance().factor, params, functions);
    return res[0];
}
LL_AUTO_STATIC_HOOK(
    TerrainShaper_jaggedness,
    HookPriority::Normal,
    "?jaggedness@TerrainShaper@@QEBAMMMM@Z",
    float,
    TerrainShaper* self,
    float          continentalness,
    float          erosion,
    float          weirdness
) {
    auto                                                ori = origin(self, continentalness, erosion, weirdness);
    std::unordered_map<std::string, std::vector<float>> params{
        {"ori",             {ori}            },
        {"continentalness", {continentalness}},
        {"erosion",         {erosion}        },
        {"weirdness",       {weirdness}      },
    };
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>>
        functions{
            {"info", [](const std::vector<std::vector<float>>& args) {
                 for (auto i : args[0]) {
                     overworld_climate_modify::OverworldClimateModify::getInstance().getSelf().getLogger().info(
                         "{}:{}",
                         __FUNCSIG__,
                         i
                     );
                 }
                 return args[0];
             }}
    };
    auto res = expr::eval(climate_modify_config::Config::instance().factor, params, functions);
    return res[0];
}

LL_AUTO_STATIC_HOOK(
    TerrainShaper_offset,
    HookPriority::Normal,
    "?offset@TerrainShaper@@QEBAMMMM@Z",
    float,
    TerrainShaper* self,
    float          continentalness,
    float          erosion,
    float          weirdness
) {
    auto                                                ori = origin(self, continentalness, erosion, weirdness);
    std::unordered_map<std::string, std::vector<float>> params{
        {"ori",             {ori}            },
        {"continentalness", {continentalness}},
        {"erosion",         {erosion}        },
        {"weirdness",       {weirdness}      },
    };
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>>
        functions{
            {"info", [](const std::vector<std::vector<float>>& args) {
                 for (auto i : args[0]) {
                     overworld_climate_modify::OverworldClimateModify::getInstance().getSelf().getLogger().info(
                         "{}:{}",
                         __FUNCSIG__,
                         i
                     );
                 }
                 return args[0];
             }}
    };
    auto res = expr::eval(climate_modify_config::Config::instance().factor, params, functions);
    return res[0];
}
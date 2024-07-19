#pragma once
#include <string>
namespace climate_modify_config {
struct Config {
    int           version = 1;
    std::string   factor = "ori;", jaggedness = "ori;", offset = "ori;";
    static Config instance();
};
} // namespace climate_modify_config
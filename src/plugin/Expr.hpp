#pragma once
#include <functional>
#include <string>
#include <unordered_map>

namespace expr {
std::vector<float> eval(
    const std::string&                                                                                         code,
    const std::unordered_map<std::string, std::vector<float>>&                                                 params,
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>> functions
);
}
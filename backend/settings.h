#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "utils.h"

class Settings {
private:
    std::filesystem::path SETTINGS_FP;
    const std::string SETTINGS_DIR = ".QtFiles";
    const std::string SETTINGS_FILENAME = "settings.json";

    nlohmann::json settings_json;
    void initSettings();

public:
    Settings();
    std::vector<std::pair<std::string, std::string>> loadSettings();
    std::string getSetting(const std::string& setting);
    void setSetting(const std::string& key, const std::string& value);
};

#include "settings.h"
#include "../utils/utils.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;

Settings::Settings() {
    this->SETTINGS_FP = getHomeDirectory() + separator() + this->SETTINGS_DIR + separator() + this->SETTINGS_FILENAME;
    this->initSettings();
}

void Settings::initSettings() {
    if (std::filesystem::exists(this->SETTINGS_FP)) {
        std::ifstream file(this->SETTINGS_FP);
        file >> settings_json;
        return;
    }

    std::filesystem::create_directories(getHomeDirectory() + separator() + SETTINGS_DIR);

    // Init values
    settings_json = {
        {"bg_color", "#000000"},
        {"font_color", "#ffffff"},
        {"selected_bg_color", "#cccccc"},
        {"font_size", 13},
        {"item_height", 25},
        {"icon_size", 16}
    };
    std::ofstream file(this->SETTINGS_FP);
    file << settings_json.dump(4);
}

//Not used
std::vector<std::pair<std::string, std::string>> Settings::loadSettings() {
    std::vector<std::pair<std::string, std::string>> result;
    if (std::filesystem::exists(this->SETTINGS_FP)) {
        std::ifstream file(this->SETTINGS_FP);
        file >> settings_json;
        for (auto& [k, v] : settings_json.items()) {
            result.emplace_back(k, v.dump());
        }
    }
    return result;
}

std::string Settings::getSetting(const std::string& setting) {
    if (settings_json.contains(setting)) {
        return settings_json[setting].dump();
    }
    return "";
}

void Settings::setSetting(const std::string& key, const std::string& value) {
    try {
        settings_json[key] = json::parse(value);
    } catch (const json::parse_error& e) {
        settings_json[key] = value;
    }

    std::ofstream file(this->SETTINGS_FP);
    file << settings_json.dump(4);
}

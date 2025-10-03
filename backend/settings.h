#pragma once
#include <string>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>

class Settings {
private:
    std::string SETTINGS_FP;
    const std::string SETTINGS_DIR = ".QtFiles";
    const std::string SETTINGS_FILENAME = "settings.json";

    void init_settings();
    nlohmann::json settings_json;

public:
    Settings();
    std::vector<std::pair<std::string, std::string>> load_settings();
    std::string get_setting(const std::string& setting);
    void set_setting(const std::string& key, const std::string& value);
};

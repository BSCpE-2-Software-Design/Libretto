#include "FilterEngine.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <nlohmann/json.hpp>

void FilterEngine::addRule(const Rule& rule) {
    rules.push_back(rule);
}

bool FilterEngine::loadPresetLibrary(const std::string& filename) {
    try {
        std::ifstream in(filename);
        if (!in) return false;
        nlohmann::json j;
        in >> j;
        presets.clear();

        if (j.is_object() && j.contains("presets") && j["presets"].is_array()) {
            for (const auto& p : j["presets"]) {
                if (!p.is_object()) continue;
                std::string name = p.value("name", std::string());
                std::vector<Rule> rs;
                if (p.contains("rules") && p["rules"].is_array()) {
                    for (const auto& r : p["rules"]) rs.push_back(Rule::fromJson(r));
                }
                if (name.empty()) name = "preset" + std::to_string(presets.size()+1);
                presets.emplace_back(name, rs);
            }
            return true;
        }

        // Support named top-level presets: { "Artist A": [rules], "Artist B": { "rules": [...] } }
        if (j.is_object()) {
            for (auto it = j.begin(); it != j.end(); ++it) {
                const std::string key = it.key();
                if (key == "rules" || key == "presets") continue;
                const auto& val = it.value();
                std::vector<Rule> rs;
                if (val.is_array()) {
                    for (const auto& r : val) rs.push_back(Rule::fromJson(r));
                } else if (val.is_object() && val.contains("rules") && val["rules"].is_array()) {
                    for (const auto& r : val["rules"]) rs.push_back(Rule::fromJson(r));
                }
                if (!rs.empty()) presets.emplace_back(key, rs);
            }
            return !presets.empty();
        }

        return false;
    } catch (...) {
        return false;
    }
}

bool FilterEngine::applyPresetMatchingInput(const std::string& input) {
    if (presets.empty()) return false;
    std::string inLower = input;
    std::transform(inLower.begin(), inLower.end(), inLower.begin(), ::tolower);

    // First match by preset name
    for (const auto& p : presets) {
        std::string nameLower = p.first;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        if (nameLower.find(inLower) != std::string::npos) {
            // apply
            rules = p.second;
            return true;
        }
    }

    // Then match by rule values inside presets
    for (const auto& p : presets) {
        for (const auto& r : p.second) {
            std::string field = r.toString();
            // We need a better way: inspect rule internals via toJson
            auto j = r.toJson();
            std::string f = j.value("field", std::string());
            std::string v = j.value("value", std::string());
            std::string vLower = v;
            std::transform(vLower.begin(), vLower.end(), vLower.begin(), ::tolower);
            if ((f == "artist" || f == "genre") && vLower.find(inLower) != std::string::npos) {
                rules = p.second; return true;
            }
            if (f == "energy") {
                std::string op = j.value("op", std::string());
                int val = 0;
                try { val = std::stoi(j.value("value", std::string("0"))); } catch(...) {}
                if (inLower.find("relapse") != std::string::npos && op == "<=" && val <= 5) { rules = p.second; return true; }
            }
        }
    }

    return false;
}

bool FilterEngine::saveRules(const std::string& filename) const {
    try {
        nlohmann::json j = nlohmann::json::array();
        for (const auto& r : rules) j.push_back(r.toJson());
        std::ofstream out(filename);
        if (!out) return false;
        out << j.dump(4);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool FilterEngine::loadRules(const std::string& filename) {
    try {
        std::ifstream in(filename);
        if (!in) return false;
        nlohmann::json j;
        in >> j;
        rules.clear();

        // Top-level array of rules
        if (j.is_array()) {
            for (const auto& item : j) {
                rules.push_back(Rule::fromJson(item));
            }
            return true;
        }

        // Object formats
        if (j.is_object()) {
            // Simple object with "rules" array
            if (j.contains("rules") && j["rules"].is_array()) {
                for (const auto& item : j["rules"]) rules.push_back(Rule::fromJson(item));
                return true;
            }

            // Presets array: prompt user to choose which preset to load
            if (j.contains("presets") && j["presets"].is_array() && !j["presets"].empty()) {
                std::vector<const nlohmann::json*> presets;
                std::vector<std::string> names;
                for (const auto& p : j["presets"]) {
                    if (p.is_object() && p.contains("rules") && p["rules"].is_array()) {
                        presets.push_back(&p);
                        std::string name = p.value("name", std::string());
                        if (name.empty()) name = "Preset " + std::to_string(presets.size());
                        names.push_back(name);
                    }
                }

                if (presets.empty()) return false;

                // If only one preset, load it
                if (presets.size() == 1) {
                    for (const auto& item : (*presets[0])["rules"]) rules.push_back(Rule::fromJson(item));
                    return true;
                }

                // Multiple presets: list and ask user to choose
                std::cout << "Multiple presets found in '" << filename << "':\n";
                for (size_t i = 0; i < names.size(); ++i) {
                    std::cout << "  " << (i + 1) << ") " << names[i] << "\n";
                }
                std::cout << "Enter preset number or name to load (Enter for 1): ";
                std::string choice;
                std::getline(std::cin, choice);
                if (choice.empty()) choice = "1";

                int chosenIndex = -1;
                // try parse as number
                try {
                    size_t pos = 0;
                    int idx = std::stoi(choice, &pos);
                    if (pos > 0) {
                        if (idx >= 1 && idx <= (int)presets.size()) chosenIndex = idx - 1;
                    }
                } catch(...) { /* not a number */ }

                if (chosenIndex == -1) {
                    // try match by name (case-insensitive)
                    std::string choiceLower = choice;
                    std::transform(choiceLower.begin(), choiceLower.end(), choiceLower.begin(), ::tolower);
                    for (size_t i = 0; i < names.size(); ++i) {
                        std::string n = names[i];
                        std::string nLower = n;
                        std::transform(nLower.begin(), nLower.end(), nLower.begin(), ::tolower);
                        if (nLower == choiceLower) { chosenIndex = (int)i; break; }
                    }
                }

                if (chosenIndex < 0 || chosenIndex >= (int)presets.size()) {
                    std::cerr << "Invalid preset selection. Aborting load.\n";
                    return false;
                }

                for (const auto& item : (*presets[chosenIndex])["rules"]) rules.push_back(Rule::fromJson(item));
                return true;
            }

            // Support object where each key is a named preset with an array or object with "rules"
            bool anyPreset = false;
            std::vector<std::pair<std::string, const nlohmann::json*>> mapPresets;
            for (auto it = j.begin(); it != j.end(); ++it) {
                const auto& val = it.value();
                if (val.is_array()) {
                    mapPresets.emplace_back(it.key(), &val);
                    anyPreset = true;
                } else if (val.is_object() && val.contains("rules") && val["rules"].is_array()) {
                    mapPresets.emplace_back(it.key(), &val["rules"]);
                    anyPreset = true;
                }
            }

            if (anyPreset) {
                if (mapPresets.size() == 1) {
                    const auto& arr = *mapPresets[0].second;
                    for (const auto& item : arr) rules.push_back(Rule::fromJson(item));
                    return true;
                }
                std::cout << "Multiple named presets found in '" << filename << "':\n";
                for (size_t i = 0; i < mapPresets.size(); ++i) {
                    std::cout << "  " << (i + 1) << ") " << mapPresets[i].first << "\n";
                }
                std::cout << "Enter preset number or name to load (Enter for 1): ";
                std::string choice;
                std::getline(std::cin, choice);
                if (choice.empty()) choice = "1";

                int chosenIndex = -1;
                try {
                    size_t pos = 0;
                    int idx = std::stoi(choice, &pos);
                    if (pos > 0) {
                        if (idx >= 1 && idx <= (int)mapPresets.size()) chosenIndex = idx - 1;
                    }
                } catch(...) { }

                if (chosenIndex == -1) {
                    std::string choiceLower = choice;
                    std::transform(choiceLower.begin(), choiceLower.end(), choiceLower.begin(), ::tolower);
                    for (size_t i = 0; i < mapPresets.size(); ++i) {
                        std::string n = mapPresets[i].first;
                        std::string nLower = n;
                        std::transform(nLower.begin(), nLower.end(), nLower.begin(), ::tolower);
                        if (nLower == choiceLower) { chosenIndex = (int)i; break; }
                    }
                }
                if (chosenIndex < 0 || chosenIndex >= (int)mapPresets.size()) {
                    std::cerr << "Invalid preset selection. Aborting load.\n";
                    return false;
                }
                const auto& arr = *mapPresets[chosenIndex].second;
                for (const auto& item : arr) rules.push_back(Rule::fromJson(item));
                return true;
            }

            // Unsupported object format
            return false;
        }

        // Other JSON types unsupported
        return false;
    }
    catch (...) {
        return false;
    }
}

void FilterEngine::clearRules() {
    rules.clear();
}

std::vector<Track> FilterEngine::apply(const std::vector<Track>& tracks) const {
    std::vector<Track> filtered;
    for (const auto& track : tracks) {
        bool matchesAll = true;
        for (const auto& rule : rules) {
            if (!rule.matches(track)) {
                matchesAll = false;
                break;
            }
        }
        if (matchesAll) {
            filtered.push_back(track);
        }
    }
    return filtered;
}
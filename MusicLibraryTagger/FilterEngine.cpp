#include "FilterEngine.h"
#include <fstream>
#include <nlohmann/json.hpp>

void FilterEngine::addRule(const Rule& rule) {
    rules.push_back(rule);
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
        // Support both a top-level array of rules and an object with a "rules" array
        if (j.is_array()) {
            for (const auto& item : j) {
                rules.push_back(Rule::fromJson(item));
            }
        }
        else if (j.is_object()) {
            if (j.contains("rules") && j["rules"].is_array()) {
                for (const auto& item : j["rules"]) rules.push_back(Rule::fromJson(item));
            }
            // Also support a simple presets file where each preset has "rules"; load the first preset's rules
            else if (j.contains("presets") && j["presets"].is_array() && !j["presets"].empty()) {
                const auto& first = j["presets"][0];
                if (first.contains("rules") && first["rules"].is_array()) {
                    for (const auto& item : first["rules"]) rules.push_back(Rule::fromJson(item));
                }
            }
            else {
                // Unsupported format
                return false;
            }
        }
        else {
            return false;
        }
        return true;
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
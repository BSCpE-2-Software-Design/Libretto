#include "Rule.h"
#include <algorithm>
#include <cctype>
#include <nlohmann/json.hpp>

Rule::Rule(const std::string& f, const std::string& o, const std::string& v)
    : field(f), op(o), value(v) {
}

bool Rule::matches(const Track& track) const {
    std::string trackValue;
    int trackInt = 0;

    // Special field that means "match any text field" (title, artist, genre, mood)
    auto anyFieldContains = [&](const std::string& valLower) {
        std::string t;
        t = track.getTitle(); std::transform(t.begin(), t.end(), t.begin(), ::tolower); if (t.find(valLower) != std::string::npos) return true;
        t = track.getArtist(); std::transform(t.begin(), t.end(), t.begin(), ::tolower); if (t.find(valLower) != std::string::npos) return true;
        t = track.getGenre(); std::transform(t.begin(), t.end(), t.begin(), ::tolower); if (t.find(valLower) != std::string::npos) return true;
        t = track.getMood(); std::transform(t.begin(), t.end(), t.begin(), ::tolower); if (t.find(valLower) != std::string::npos) return true;
        return false;
    };

    if (field == "title" || field == "artist" || field == "genre" || field == "mood" || field == "any") {
        if (field == "title") trackValue = track.getTitle();
        else if (field == "artist") trackValue = track.getArtist();
        else if (field == "genre") trackValue = track.getGenre();
        else if (field == "mood") trackValue = track.getMood();

        std::string trackLower = trackValue;
        std::string valueLower = value;
        std::transform(trackLower.begin(), trackLower.end(), trackLower.begin(), ::tolower);
        std::transform(valueLower.begin(), valueLower.end(), valueLower.begin(), ::tolower);

        // If field == "any" then use anyFieldContains helper for contains/op checks
        if (field == "any") {
            if (op == "contains") return anyFieldContains(valueLower);
            if (op == "==") return anyFieldContains(valueLower); // treat == as any-field contains or equals
            if (op == "!=") return !anyFieldContains(valueLower);
            return false;
        }
        if (op == "==") return trackLower == valueLower;
        if (op == "!=") return trackLower != valueLower;
        if (op == "contains") return trackLower.find(valueLower) != std::string::npos;
    }
    else if (field == "energy") {
        trackInt = track.getEnergy();
        int valInt = std::stoi(value);
        if (op == "==") return trackInt == valInt;
        if (op == "!=") return trackInt != valInt;
        if (op == "<=") return trackInt <= valInt;
        if (op == ">=") return trackInt >= valInt;
        if (op == "<") return trackInt < valInt;
        if (op == ">") return trackInt > valInt;
    }

    return false;
}

std::string Rule::toString() const {
    return field + " " + op + " " + value;
}

nlohmann::json Rule::toJson() const {
    nlohmann::json j;
    j["field"] = field;
    j["op"] = op;
    j["value"] = value;
    return j;
}

Rule Rule::fromJson(const nlohmann::json& j) {
    std::string f = j.value("field", "");
    std::string o = j.value("op", "");
    std::string v = j.value("value", "");
    return Rule(f, o, v);
}
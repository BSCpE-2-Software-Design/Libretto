#include "Rule.h"
#include <algorithm>
#include <cctype>

Rule::Rule(const std::string& f, const std::string& o, const std::string& v)
    : field(f), op(o), value(v) {
}

bool Rule::matches(const Track& track) const {
    std::string trackValue;
    int trackInt = 0;

    if (field == "title" || field == "artist" || field == "genre" || field == "mood") {
        if (field == "title") trackValue = track.getTitle();
        else if (field == "artist") trackValue = track.getArtist();
        else if (field == "genre") trackValue = track.getGenre();
        else if (field == "mood") trackValue = track.getMood();

        std::string trackLower = trackValue;
        std::string valueLower = value;
        std::transform(trackLower.begin(), trackLower.end(), trackLower.begin(), ::tolower);
        std::transform(valueLower.begin(), valueLower.end(), valueLower.begin(), ::tolower);

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
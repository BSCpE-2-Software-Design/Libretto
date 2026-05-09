#include "FilterEngine.h"

void FilterEngine::addRule(const Rule& rule) {
    rules.push_back(rule);
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
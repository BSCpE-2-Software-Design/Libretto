#pragma once
#include "Track.h"
#include "Rule.h"
#include <vector>

class FilterEngine {
private:
    std::vector<Rule> rules;

public:
    void addRule(const Rule& rule);
    void clearRules();
    std::vector<Track> apply(const std::vector<Track>& tracks) const;
    std::vector<Rule> getRules() const { return rules; }
    bool hasRules() const { return !rules.empty(); }
    bool saveRules(const std::string& filename) const;
    bool loadRules(const std::string& filename);
};

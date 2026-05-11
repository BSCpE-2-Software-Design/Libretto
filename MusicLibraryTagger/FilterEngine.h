#pragma once
#include "Track.h"
#include "Rule.h"
#include <vector>

class FilterEngine {
private:
    std::vector<Rule> rules;
    // Loaded presets from an external JSON file (name -> rules)
    std::vector<std::pair<std::string, std::vector<Rule>>> presets;

public:
    void addRule(const Rule& rule);
    void clearRules();
    std::vector<Track> apply(const std::vector<Track>& tracks) const;
    std::vector<Rule> getRules() const { return rules; }
    bool hasRules() const { return !rules.empty(); }
    bool saveRules(const std::string& filename) const;
    bool loadRules(const std::string& filename);
    // Load a preset library (F_Rule.json) into memory
    bool loadPresetLibrary(const std::string& filename);
    // Try to find and apply a preset matching user input (artist/genre/relapse)
    bool applyPresetMatchingInput(const std::string& input);
};

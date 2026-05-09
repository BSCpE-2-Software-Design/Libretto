#pragma once
#include "Track.h"
#include <string>
#include <functional>

class Rule {
private:
    std::string field;
    std::string op;
    std::string value;

public:
    Rule(const std::string& f, const std::string& o, const std::string& v);

    bool matches(const Track& track) const;
    std::string toString() const;
};

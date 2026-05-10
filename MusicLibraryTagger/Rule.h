#pragma once
#include "Track.h"
#include <string>
#include <functional>
#include <nlohmann/json.hpp>

class Rule {
private:
    std::string field;
    std::string op;
    std::string value;

public:
    Rule(const std::string& f, const std::string& o, const std::string& v);

    nlohmann::json toJson() const;
    static Rule fromJson(const nlohmann::json& j);

    bool matches(const Track& track) const;
    std::string toString() const;
};

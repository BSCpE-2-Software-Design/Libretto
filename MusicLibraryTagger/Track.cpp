#include "Track.h"
#include <sstream>

Track::Track(const std::string& t, const std::string& a, const std::string& g,
    const std::string& m, int e, const std::string& fp)
    : title(t), artist(a), genre(g), mood(m), energy(e), filePath(fp) {
}

nlohmann::json Track::toJson() const {
    return {
        {"title", title},
        {"artist", artist},
        {"genre", genre},
        {"mood", mood},
        {"energy", energy},
        {"filePath", filePath}
    };
}

Track Track::fromJson(const nlohmann::json& json) {
    // Use tolerant parsing so missing/extra fields don't crash the UI.
    return Track(
        json.value("title", std::string{}),
        json.value("artist", std::string{}),
        json.value("genre", std::string{}),
        json.value("mood", std::string{}),
        json.value("energy", 0),
        json.value("filePath", std::string{})
    );
}

std::string Track::toString() const {
    std::stringstream ss;
    ss << title << " - " << artist;
    return ss.str();
}
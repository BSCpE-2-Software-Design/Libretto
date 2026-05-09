#pragma once
#include "Track.h"
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class Library {
private:
    std::vector<Track> tracks;

public:
    void loadFromJson(const std::string& filename);
    std::vector<Track> getAllTracks() const { return tracks; }
    size_t size() const { return tracks.size(); }
    Track getTrack(size_t index) const;
};

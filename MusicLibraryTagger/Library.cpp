#include "Library.h"
#include <fstream>
#include <iostream>
#include <unordered_set>

void Library::loadFromJson(const std::string& filename) {
    tracks.clear();

    // Load main library file if it exists
    std::vector<Track> mainTracks;
    std::ifstream inFile(filename);
    if (inFile) {
        try {
            nlohmann::json j;
            inFile >> j;
            if (j.is_array()) {
                for (const auto& item : j) {
                    mainTracks.push_back(Track::fromJson(item));
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to read library '" << filename << "': " << e.what() << std::endl;
        }
    }

    // Load PLAYLIST.json if present and ensure those tracks appear first
    std::vector<Track> playlistTracks;
    std::ifstream plFile("PLAYLIST.json");
    if (plFile) {
        try {
            nlohmann::json pj;
            plFile >> pj;
            if (pj.is_array()) {
                for (const auto& item : pj) {
                    playlistTracks.push_back(Track::fromJson(item));
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to read PLAYLIST.json: " << e.what() << std::endl;
        }
    }

    // Combine: playlist tracks first (preserve order), then remaining main tracks without duplicates
    std::unordered_set<std::string> seen;
    for (const auto& t : playlistTracks) {
        tracks.push_back(t);
        seen.insert(t.getFilePath());
    }
    for (const auto& t : mainTracks) {
        if (seen.find(t.getFilePath()) == seen.end()) {
            tracks.push_back(t);
            seen.insert(t.getFilePath());
        }
    }
}

Track Library::getTrack(size_t index) const {
    if (index < tracks.size()) return tracks[index];
    throw std::out_of_range("Track index out of range");
}

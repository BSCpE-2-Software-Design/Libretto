#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Track {
private:
    std::string title;
    std::string artist;
    std::string genre;
    std::string mood;
    int energy;
    std::string filePath;

public:
    Track() = default;
    Track(const std::string& t, const std::string& a, const std::string& g,
        const std::string& m, int e, const std::string& fp);

    // Getters
    std::string getTitle() const { return title; }
    std::string getArtist() const { return artist; }
    std::string getGenre() const { return genre; }
    std::string getMood() const { return mood; }
    int getEnergy() const { return energy; }
    std::string getFilePath() const { return filePath; }

    // JSON serialization
    nlohmann::json toJson() const;
    static Track fromJson(const nlohmann::json& json);

    std::string toString() const;
};


#pragma once
#include "Track.h"
#include "FilterEngine.h"
#include "Library.h"
#include <vector>
#include <string>

class BrowserView {
private:
    Library& library;
    FilterEngine& filterEngine;
    std::vector<Track> currentView;
    int selectedIndex;
    int scrollOffset;
    const int maxVisible = 15;
    // Playback simulation
    bool playing;
    int playbackPos;
    const int playbackBarLen = 35;

    void renderHeader();
    void renderNowPlaying();
    void renderTracks();
    void renderFooter();
    void updateCurrentView();

public:
    BrowserView(Library& lib, FilterEngine& fe);
    void render();
    void moveUp();
    void moveDown();
    void addFilter();
    void clearFilters();
    void savePlaylist();
    char getInput();
    // Return the file path of the currently selected track or empty string if none
    std::string getSelectedFilePath() const;
    // Toggle playback (play / pause) for the selected track
    void togglePlay();
    bool isPlaying() const;
};


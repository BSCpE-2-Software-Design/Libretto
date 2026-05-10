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

    void renderHeader();
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
};


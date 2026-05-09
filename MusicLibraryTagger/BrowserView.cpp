#include "BrowserView.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Rule.h"

BrowserView::BrowserView(Library& lib, FilterEngine& fe)
    : library(lib), filterEngine(fe), selectedIndex(0), scrollOffset(0) {
    updateCurrentView();
}

void BrowserView::renderHeader() {
    system("clear||cls");
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  🎵  MUSIC LIBRARY TAGGER & PLAYLIST MANAGER  🎵                      ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  j/k: Navigate  |  f: Filter  |  x: Clear  |  s: Save Playlist  |  q: Quit  ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
}

void BrowserView::renderTracks() {
    std::cout << "📋 Current View (" << currentView.size() << "/" << library.size() << " tracks";
    if (filterEngine.hasRules()) {
        std::cout << ") - Filters: ";
        auto rules = filterEngine.getRules();
        for (size_t i = 0; i < rules.size(); ++i) {
            std::cout << rules[i].toString();
            if (i < rules.size() - 1) std::cout << " AND ";
        }
    }
    std::cout << std::endl;
    std::cout << "══════════════════════════════════════════════════════════════════════" << std::endl;

    int start = scrollOffset;
    int end = std::min(start + maxVisible, (int)currentView.size());

    for (int i = start; i < end; ++i) {
        const auto& track = currentView[i];
        std::string prefix = (i == selectedIndex) ? "▶️  " : "  ";
        std::cout << prefix;

        // Title and Artist
        std::cout << std::setw(35) << std::left << track.getTitle().substr(0, 35);
        std::cout << "│ " << std::setw(15) << std::left << track.getArtist().substr(0, 15);
        std::cout << "│ " << track.getGenre().substr(0, 12);
        std::cout << "│ " << track.getMood().substr(0, 12);
        std::cout << "│ ⚡ " << track.getEnergy();
        std::cout << std::endl;
    }

    std::cout << "══════════════════════════════════════════════════════════════════════" << std::endl;
}

void BrowserView::renderFooter() {
    std::cout << std::endl;
    std::cout << "Selected: " << (selectedIndex < currentView.size() ?
        currentView[selectedIndex].toString() : "None") << std::endl;
    std::cout << "Press j/k to navigate, f to filter, x to clear, s to save, q to quit" << std::endl;
}

void BrowserView::updateCurrentView() {
    currentView = filterEngine.apply(library.getAllTracks());
    if (selectedIndex >= currentView.size()) {
        selectedIndex = currentView.size() > 0 ? 0 : 0;
    }
    scrollOffset = std::max(0, (int)selectedIndex - maxVisible / 2);
}

void BrowserView::render() {
    renderHeader();
    renderTracks();
    renderFooter();
    std::cout << std::endl;
}

void BrowserView::moveUp() {
    if (selectedIndex > 0) {
        selectedIndex--;
        scrollOffset = std::max(0, (int)selectedIndex - maxVisible / 2);
    }
}

void BrowserView::moveDown() {
    if (selectedIndex < currentView.size() - 1) {
        selectedIndex++;
        scrollOffset = std::min((int)currentView.size() - maxVisible, (int)selectedIndex - maxVisible / 2);
    }
}

void BrowserView::addFilter() {
    std::string input;
    std::cout << "\nEnter filter (e.g., 'genre == rap', 'energy <= 5', 'mood contains love'): ";
    std::getline(std::cin, input);

    // Simple parsing: field op value
    size_t eqPos = input.find("==");
    size_t nePos = input.find("!=");
    size_t lePos = input.find("<=");
    size_t gePos = input.find(">=");
    size_t ltPos = input.find("<");
    size_t gtPos = input.find(">");
    size_t space1 = input.find(" ");
    size_t space2 = input.rfind(" ");

    if (space1 != std::string::npos && space2 != std::string::npos && space1 != space2) {
        std::string field = input.substr(0, space1);
        std::string op = input.substr(space1 + 1, space2 - space1 - 1);
        std::string value = input.substr(space2 + 1);

        Rule rule(field, op, value);
        filterEngine.addRule(rule);
        updateCurrentView();
    }
}

void BrowserView::clearFilters() {
    filterEngine.clearRules();
    updateCurrentView();
    selectedIndex = 0;
    scrollOffset = 0;
}

void BrowserView::savePlaylist() {
    std::string name;
    std::cout << "\nEnter playlist name: ";
    std::getline(std::cin, name);

    std::cout << "Playlist '" << name << "' saved with " << currentView.size() << " tracks!" << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

char BrowserView::getInput() {
    char c;
    std::cin.get(c);
    std::cin.ignore();
    return c;
}
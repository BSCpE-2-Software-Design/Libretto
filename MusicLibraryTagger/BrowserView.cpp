#include "BrowserView.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Rule.h"

BrowserView::BrowserView(Library& lib, FilterEngine& fe)
    : library(lib), filterEngine(fe), selectedIndex(-1), scrollOffset(0) {
    updateCurrentView();
}

void BrowserView::renderHeader() {
    system("clear||cls");
    std::cout << "\n";
    std::cout << "+------------------------------------------------------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                 LIBRETTO                                                   |" << std::endl;
    std::cout << "+------------------------------------------------------------------------------------------------------------+" << std::endl;
    std::cout << "|               Song Title            |            Artist             |    Genre    |      Mood      |Energy |" << std::endl;
    std::cout << "+------------------------------------------------------------------------------------------------------------+" << std::endl;
    std::cout << std::endl;
}

void BrowserView::renderTracks() {
    std::cout << "[ Current View (" << currentView.size() << "/" << library.size() << " tracks)";
    if (filterEngine.hasRules()) {
        std::cout << ") - Filters: ";
        auto rules = filterEngine.getRules();
        for (size_t i = 0; i < rules.size(); ++i) {
            std::cout << rules[i].toString();
            if (i < rules.size() - 1) std::cout << " AND ";
        }
    }
    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;

    int start = scrollOffset;
    int end = std::min(start + maxVisible, (int)currentView.size());

    for (int i = start; i < end; ++i) {
        const auto& track = currentView[i];
        std::string prefix = (i == selectedIndex) ? "-> " : "   ";
        std::cout << prefix;

        // Title and Artist
        std::cout << std::setw(35) << std::left << track.getTitle().substr(0, 15);
        std::cout << "| " << std::setw(30) << std::left << track.getArtist().substr(0, 35);
        std::cout << "| " << std::setw(13) << std::left << track.getGenre() ;
        std::cout << "| " << std::setw(15) << std::left << track.getMood();
        std::cout << "| " << std::setw(5) << std::left << track.getEnergy() << " | ";
        std::cout << std::endl;
    }

    std::cout << "----------------------------------------------------------------------" << std::endl;
}

void BrowserView::renderFooter() {
    std::cout << std::endl;
    std::string sel = "None";
    if (selectedIndex >= 0 && selectedIndex < (int)currentView.size()) {
        sel = currentView[selectedIndex].toString();
    }
    std::cout << "Selected: " << sel << std::endl;
    std::cout << "Press j/k to navigate, f to filter, x to clear, s to save, q to quit" << std::endl;
}

void BrowserView::updateCurrentView() {
    currentView = filterEngine.apply(library.getAllTracks());
    if (currentView.empty()) {
        selectedIndex = -1;
    } else if (selectedIndex >= (int)currentView.size()) {
        selectedIndex = 0;
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
    // If there are no tracks, nothing to do
    if (currentView.empty()) return;

    // If nothing is selected yet, pressing 'j' should start the selection
    // from the first item (index 0 — e.g., "sirena").
    if (selectedIndex < 0) {
        selectedIndex = 0;
    }
    // Normal move up: go to previous item if possible
    else if (selectedIndex > 0) {
        selectedIndex--;
    }
    // if already at 0, stay at 0

    scrollOffset = std::max(0, (int)selectedIndex - maxVisible / 2);
}

void BrowserView::moveDown() {
    if (currentView.empty()) return;
    if (selectedIndex < 0) {
        selectedIndex = 0;
    }
    else if (selectedIndex < (int)currentView.size() - 1) {
        selectedIndex++;
    }
    scrollOffset = std::max(0, std::min((int)currentView.size() - maxVisible, (int)selectedIndex - maxVisible / 2));
}

void BrowserView::addFilter() {
    std::string input;
    std::cout << "\nEnter filter (e.g., 'genre == rap', 'energy <= 5', 'mood contains love'): ";
    std::getline(std::cin, input);
    // Trim
    auto ltrim = [](std::string &s) { s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); })); };
    auto rtrim = [](std::string &s) { s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end()); };
    ltrim(input); rtrim(input);

    if (input.empty()) {
        std::cout << "No filter entered. To load rules from a file enter the filename (e.g., F_Rule.json) or a keyword to search." << std::endl;
        return;
    }

    // If user provided a JSON filename, try to load rules from it
    if (input.size() > 5 && input.substr(input.size() - 5) == ".json") {
        bool ok = filterEngine.loadRules(input);
        if (!ok) std::cout << "Failed to load rules from " << input << std::endl;
        else updateCurrentView();
        return;
    }

    // Simple parsing: field op value (legacy support)
    size_t space1 = input.find(' ');
    size_t space2 = input.rfind(' ');
    if (space1 != std::string::npos && space2 != std::string::npos && space1 != space2) {
        std::string field = input.substr(0, space1);
        std::string op = input.substr(space1 + 1, space2 - space1 - 1);
        std::string value = input.substr(space2 + 1);

        Rule rule(field, op, value);
        filterEngine.addRule(rule);
        updateCurrentView();
        return;
    }

    // Treat single keyword as a search across common text fields (title/artist/genre/mood)
    Rule anyRule("any", "contains", input);
    filterEngine.addRule(anyRule);
    updateCurrentView();
}

void BrowserView::clearFilters() {
    filterEngine.clearRules();
    updateCurrentView();
    selectedIndex = -1;
    scrollOffset = 0;
}

void BrowserView::savePlaylist() {
    std::string name;
    // Save current filter rules to a JSON file and then clear filters so all tracks display
    if (filterEngine.hasRules()) {
        bool ok = filterEngine.saveRules("rules.json");
        if (!ok) std::cout << "Failed to save rules to rules.json" << std::endl;
        else std::cout << "Rules saved to rules.json" << std::endl;
    } else {
        std::cout << "No rules to save." << std::endl;
    }
    std::cout << "Press Enter to continue...";
    std::cin.get();
    filterEngine.clearRules();
    updateCurrentView();
}

char BrowserView::getInput() {
    char c;
    std::cin.get(c);
    std::cin.ignore();
    return c;
}
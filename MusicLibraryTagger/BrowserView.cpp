#include "BrowserView.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Rule.h"

BrowserView::BrowserView(Library& lib, FilterEngine& fe)
    : library(lib), filterEngine(fe), selectedIndex(-1), scrollOffset(0) {
    playing = false;
    playbackPos = 0;
    filterEngine.loadPresetLibrary("F_Rule.json");
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
    std::cout << " Current View (" << currentView.size() << "/" << library.size() << " tracks)";
    if (filterEngine.hasRules()) {
        std::cout << ") - Filters: ";
        auto rules = filterEngine.getRules();
        for (size_t i = 0; i < rules.size(); ++i) {
            std::cout << rules[i].toString();
            if (i < rules.size() - 1) std::cout << " AND ";
        }
    }
    std::cout << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------------+" << std::endl;

    int start = scrollOffset;
    int end = std::min(start + maxVisible, (int)currentView.size());

    for (int i = start; i < end; ++i) {
        const auto& track = currentView[i];
        std::string prefix = (i == selectedIndex) ? "-> " : "   ";
        std::cout << prefix;

        // Title and Artist
        std::cout << std::setw(35) << std::left << track.getTitle().substr(0, 15);
        std::cout << "| " << std::setw(30) << std::left << track.getArtist().substr(0, 35);
        std::cout << "| " << std::setw(13) << std::left << track.getGenre();
        std::cout << "| " << std::setw(15) << std::left << track.getMood();
        std::cout << "| " << std::setw(4) << std::left << track.getEnergy() << " | ";
        std::cout << std::endl;
    }

    std::cout << "-------------------------------------------------------------------------------------------------------------+" << std::endl;
}

void BrowserView::renderFooter() {
    std::cout << std::endl;


    // Playback UI
    if (isPlaying()) {
        renderNowPlaying();

    }
    std::cout << "Press j/k to navigate, f to filter, x to clear, s to save, q to quit" << std::endl;
}

void BrowserView::renderNowPlaying() {
    // draw a small boxed GUI showing now playing info
    if (selectedIndex < 0 || selectedIndex >= (int)currentView.size()) return;
    const auto& t = currentView[selectedIndex];
    std::string title = t.getTitle();
    std::string artist = t.getArtist();
    std::string fp = t.getFilePath();

    std::cout << "+---------------- Now Playing ----------------+" << std::endl;
    std::cout << "| " << std::setw(44) << std::left << (title + " - " + artist) << "|" << std::endl;
    std::cout << "| " << std::setw(44) << std::left << ("File: " + fp) << "|" << std::endl;

    // advance playback
    playbackPos = (playbackPos + 1) % (playbackBarLen + 1);
    int filled = std::min(playbackBarLen, playbackPos);
    std::cout << "| [-----------------------------------------";
    std::cout << "]" << std::setw(35) << std::left << " |" << std::endl;
    std::cout << "|                      ||                     |" << std::setw(25) << std::right << std::endl;
    std::cout << "+---------------------------------------------+" << std::endl;
}

void BrowserView::togglePlay() {
    if (currentView.empty()) return;

    // Kung wala pang selection, auto-select first track kapag nag-play
    if (selectedIndex < 0) {
        selectedIndex = 0;
    }

    playing = !playing;
    if (playing) playbackPos = 0;
}



bool BrowserView::isPlaying() const {
    return playing && selectedIndex >= 0 && selectedIndex < (int)currentView.size();
}

std::string BrowserView::getSelectedFilePath() const {
    if (selectedIndex >= 0 && selectedIndex < (int)currentView.size()) {
        return currentView[selectedIndex].getFilePath();
    }
    return std::string();
}


void BrowserView::updateCurrentView() {
    currentView = filterEngine.apply(library.getAllTracks());
    if (currentView.empty()) {
        selectedIndex = -1;
        // stop playback when no tracks
        playing = false;

    }
    else if (currentView.size()) {
        selectedIndex = -1;
        playing = true;
    }
    else if (currentView.size() == 1) {
        // If filtering yields a single track, select it automatically
        selectedIndex = 0;
        // auto-play the single filtered track
        playing = true;
    }
    else if (selectedIndex >= (int)currentView.size() || selectedIndex < 0) {
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
    if (currentView.empty()) return;
    if (selectedIndex < 0) {
        selectedIndex = 0;
    }else if (selectedIndex > 0) {
        selectedIndex--;
    }

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
    std::cout << "\nType the name of the artist, genre or mood: ";
    std::getline(std::cin, input);


    // Trim whitespace
    auto ltrim = [](std::string& s) { s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); })); };
    auto rtrim = [](std::string& s) { s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end()); };
    ltrim(input); rtrim(input);

    // Validation rules
    if (input.empty()) {
        std::cout << "[!] No filter entered.\n Tip: Entered genre (Ex. Rap, Kpop). \n";
        std::cout << "Tap Enter to continue...";
        std::cin.get();
        return;
    }
    if (std::all_of(input.begin(), input.end(), [](unsigned char c) { return std::ispunct(c); })) {
        std::cout << "[!] Invalid input: symbols are not allowed.Enter the correct word/command\n";
        std::cout << "Tap Enter to continue... " ;
        std::cin.get();
        return;
    }
    if (std::all_of(input.begin(), input.end(), ::isdigit)) {
        std::cout << "[!] Invalid input: numbers are not allowed.  \n";
        std::cout << "Tap Enter to continue... ";
        std::cin.get();
        return;
    }

    // Check capitalization for Artist/Genre/Mood
    if (input.find("artist") != std::string::npos ||
        input.find("genre") != std::string::npos ||
        input.find("mood") != std::string::npos) {
        size_t pos = input.find_last_of(' ');
        if (pos != std::string::npos && std::islower(input[pos + 1])) {
            std::cout << "[!] Invalid input: Artist/Genre/Mood must start with a capital letter.'.\n";
            std::cout << "Then tap Enter to continue...";
            std::cin.get();
            return;
        }
    }

    // Preset check
    if (filterEngine.applyPresetMatchingInput(input)) {
        updateCurrentView();
        return;
    }

    // Simple parsing: field op value
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
    else {
        std::cout << "[!] Invalid filter format. Use 'field op value'.\n";
        std::cout << "Tap Enter to continue... ";
        std::cin.get();
        return;
    }

    // Treat single keyword as search across text fields
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
    }
    else {
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

    if (c != 'j' && c != 'k' && c != 'f' && c != 's' && c != 'p' && c != 'q') {
        std::cout << "[!] Invalid command: Only j, k, f, s, p, and q are allowed.\n";
        std::cout << "Tap Enter to continue...";
        std::cin.get();
        return '\0'; 
    }

    return c;
}

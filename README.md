# Libretto
A terminal-based music library organizer with metadata tagging and dynamic, rule-based playlists.
A console TUI for organizing music libraries through structured metadata tags and interactive keyboard-driven browsing.



Class Diagram – Music Library Tagger & Playlist Manager
```mermaid
classDiagram
    class Track {
        - string title
        - string artist
        - string genre
        - string mood
        - int energy
        - string filePath
        + Track()
        + Track(t, a, g, m, e, fp)
        + getTitle() string
        + getArtist() string
        + getGenre() string
        + getMood() string
        + getEnergy() int
        + getFilePath() string
        + toJson() json
        + fromJson(json) Track
        + toString() string
    }

    class Rule {
        - string field
        - string op
        - string value
        + Rule(f, o, v)
        + toJson() json
        + fromJson(json) Rule
        + matches(track: Track) bool
        + toString() string
    }

    class Library {
        - vector<Track> tracks
        + loadFromJson(filename: string) void
        + getAllTracks() vector<Track>
        + size() size_t
        + getTrack(index: size_t) Track
    }

    class FilterEngine {
        - vector<Rule> rules
        - vector<pair<string, vector<Rule>>> presets
        + addRule(rule: Rule) void
        + clearRules() void
        + apply(tracks: vector<Track>) vector<Track>
        + getRules() vector<Rule>
        + hasRules() bool
        + saveRules(filename: string) bool
        + loadRules(filename: string) bool
        + loadPresetLibrary(filename: string) bool
        + applyPresetMatchingInput(input: string) bool
    }

    class BrowserView {
        - Library& library
        - FilterEngine& filterEngine
        - vector<Track> currentView
        - int selectedIndex
        - int scrollOffset
        - bool playing
        - int playbackPos
        - const int maxVisible
        - const int playbackBarLen
        + BrowserView(lib: Library, fe: FilterEngine)
        + render() void
        + moveUp() void
        + moveDown() void
        + addFilter() void
        + clearFilters() void
        + savePlaylist() void
        + getInput() char
        + getSelectedFilePath() string
        + togglePlay() void
        + isPlaying() bool
    }

    %% Relationships
    Library "1" --> "*" Track
    FilterEngine "1" --> "*" Rule
    BrowserView "1" --> "1" Library
    BrowserView "1" --> "1" FilterEngine
    BrowserView "1" --> "*" Track
```







Sequence Diagram 
```mermaid
sequenceDiagram
    participant User
    participant BrowserView
    participant FilterEngine
    participant Library
    participant Track

    User->>BrowserView: Start program
    BrowserView->>FilterEngine: loadPresetLibrary("F_Rule.json")
    BrowserView->>Library: getAllTracks()
    Library-->>BrowserView: return tracks
    BrowserView->>FilterEngine: apply(tracks)
    FilterEngine-->>BrowserView: filtered tracks
    BrowserView->>User: render UI (header, tracks, footer)

    User->>BrowserView: press 'f' (addFilter)
    BrowserView->>FilterEngine: addRule(rule)
    BrowserView->>Library: getAllTracks()
    Library-->>BrowserView: return tracks
    BrowserView->>FilterEngine: apply(tracks)
    FilterEngine-->>BrowserView: filtered tracks
    BrowserView->>User: updateCurrentView()

    User->>BrowserView: press 'p' (togglePlay)
    BrowserView->>Track: getFilePath()
    BrowserView->>BrowserView: playFile(filePath)
    BrowserView->>User: renderNowPlaying()

```

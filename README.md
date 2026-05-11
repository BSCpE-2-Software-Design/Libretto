# Libretto
A terminal-based music library organizer with metadata tagging and dynamic, rule-based playlists.
A console TUI for organizing music libraries through structured metadata tags and interactive keyboard-driven browsing.



Class Diagram – Music Library Tagger & Playlist Manager
```mermaid
classDiagram
    class Track {
        string title
        string artist
        string genre
        string mood
        int energy
    }

    class Library {
        +List<Track> tracks
        +void loadLibrary()
        +void addTrack(Track)
        +List<Track> getTracks()
    }

    class Rule {
        string field
        string operator
        string value
        +bool matches(Track)
    }

    class FilterEngine {
        +List<Rule> activeRules
        +List<Track> applyRules(List<Track>)
        +void clearRules()
        +void addRule(Rule)
    }

    class Playlist {
        string name
        +List<Rule> rules
        +void saveRules(List<Rule>)
        +List<Track> generate(Library)
    }

    class BrowserView {
        +void displayTracks(List<Track>)
        +void highlightTrack(Track)
        +void navigateUp()
        +void navigateDown()
    }

    class App {
        +void start()
        +void handleInput()
        +void exit()
    }

    Library "1" --> "*" Track
    FilterEngine "1" --> "*" Rule
    Playlist "1" --> "*" Rule
    FilterEngine --> Library
    BrowserView --> FilterEngine
    App --> BrowserView
    App --> FilterEngine
    App --> Library
    App --> Playlist
```




Flowchart 
```mermaid
flowchart TB
    Start([Start App]) --> LoadLib[Load or Define Library<br/>15+ Tracks, Metadata Fields]
    LoadLib --> TagTracks[Tag Tracks<br/>Title, Artist, Genre, Mood, Energy]
    TagTracks --> InitUI[Init BrowserView<br/>Keyboard Navigation]
    InitUI --> WaitInput{Wait for Keypress}

    WaitInput -->|j/k| Navigate[Move Selection Up/Down<br/>Update Highlight]
    Navigate --> WaitInput

    WaitInput -->|f| AddFilter[Add Filter<br/>Field + Operator + Value]
    AddFilter --> CreateRule[Create Rule Object]
    CreateRule --> ApplyFilter[FilterEngine.applyRules<br/>tracks + activeRules]
    ApplyFilter --> UpdateView[Update filteredTracks<br/>Refresh Screen]
    UpdateView --> WaitInput

    WaitInput -->|x| ClearFilters[Clear All Rules<br/>Show All Tracks]
    ClearFilters --> UpdateView

    WaitInput -->|s| SavePlaylist[Save activeRules]
    SavePlaylist --> StorePL[Store Playlist<br/>rules only, not songs]
    StorePL --> DisplayPL[Display Playlist<br/>Navigable View]
    DisplayPL --> WaitInput

    WaitInput -->|q| Exit([Exit App])

    subgraph Layers
        direction LR
        L1[Display Layer<br/>BrowserView] 
        L2[Filter Logic<br/>FilterEngine + Rule + Playlist]
        L3[Metadata Layer<br/>Library + Track<br/>Fields: Title, Artist, Genre, Mood, Energy]
    end

    L1 -.->|uses| L2
    L2 -.->|reads| L3

```


Sequence Diagram 
```mermaid
sequenceDiagram
    participant User
    participant UI as BrowserView (Display Layer)
    participant FilterEngine as FilterEngine (Logic Layer)
    participant Library as Library (Metadata Layer)
    participant Playlist as Playlist Store

    User->>UI: Start App
    UI->>Library: Load or Define Library (15+ Tracks, Metadata Fields)
    User->>UI: Tag Tracks (Title, Artist, Genre, Mood, Energy)
    UI->>User: Init BrowserView (Keyboard Navigation)

    loop Navigation
        User->>UI: Press "j/k" (Navigate)
        UI->>User: Update Highlight
    end

    User->>UI: Press "f" (Add Filter)
    UI->>User: Prompt for Field/Operator/Value
    User->>UI: Enter Filter Criteria
    UI->>FilterEngine: Create Rule Object
    FilterEngine->>Library: Read Track Metadata
    FilterEngine->>UI: applyRules(tracks + activeRules)
    UI->>User: Display Updated View (filteredTracks)

    User->>UI: Press "x" (Clear Filters)
    UI->>FilterEngine: Clear All Rules
    FilterEngine->>UI: Return All Tracks
    UI->>User: Display Full Library

    User->>UI: Press "s" (Save Playlist)
    UI->>FilterEngine: Collect activeRules
    FilterEngine->>Playlist: Store Playlist (rules only, not songs)
    Playlist->>UI: Confirm Playlist Stored
    UI->>User: Display Playlist (Navigable View)

    User->>UI: Press "q" (Exit App)

```

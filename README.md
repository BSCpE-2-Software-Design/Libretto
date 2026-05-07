# Libretto
A terminal-based music library organizer with metadata tagging and dynamic, rule-based playlists.
A console TUI for organizing music libraries through structured metadata tags and interactive keyboard-driven browsing.


Flowchart – Music Library Tagger & Playlist Manager
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

    WaitInput -->|s| SavePlaylist[Input Playlist Name<br/>Save activeRules]
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
    UI->>Library: Load 15+ Tracks with Metadata (Title, Artist, Genre, Mood, Energy)
    User->>UI: Tag tracks with attributes
    User->>UI: Navigate with j/k keys
    UI->>User: Highlight current track

    User->>UI: Press "f" (Add Filter)
    UI->>User: Prompt for field/operator/value
    User->>UI: Enter filter criteria (e.g., Genre == Rap)
    UI->>FilterEngine: Create Rule Object
    FilterEngine->>Library: Read track metadata
    FilterEngine->>UI: Return filteredTracks
    UI->>User: Display updated view

    User->>UI: Press "x" (Clear Filters)
    UI->>FilterEngine: Remove activeRules
    FilterEngine->>UI: Return all tracks
    UI->>User: Display full library

    User->>UI: Press "s" (Save Playlist)
    UI->>User: Prompt for playlist name
    User->>UI: Enter name
    UI->>FilterEngine: Collect activeRules
    FilterEngine->>Playlist: Save rules only
    Playlist->>UI: Confirm playlist stored
    UI->>User: Display playlist view

    User->>UI: Press "q" (Exit App)
```

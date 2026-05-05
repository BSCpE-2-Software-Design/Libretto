# Libretto
A terminal-based music library organizer with metadata tagging and dynamic, rule-based playlists.
A console TUI for organizing music libraries through structured metadata tags and interactive keyboard-driven browsing.



Flowchart - System Architecture + User Flow
```mermaid
flowchart TB
    Start([Start App]) --> LoadLib[Load Library<br/>15+ Tracks]
    LoadLib --> InitUI[Init BrowserView<br/>Show All Tracks]
    InitUI --> WaitInput{Wait for Keypress}
    
    WaitInput -->|j/k| Navigate[Move Selection Up/Down<br/>Update Highlight]
    Navigate --> WaitInput
    
    WaitInput -->|f| AddFilter[Filter Flow:<br/>1. Input field<br/>2. Input operator<br/>3. Input value]
    AddFilter --> CreateRule[Create Rule Object]
    CreateRule --> ApplyFilter[FilterEngine.applyRules<br/>tracks + activeRules]
    ApplyFilter --> UpdateView[Update filteredTracks<br/>Refresh Screen]
    UpdateView --> WaitInput
    
    WaitInput -->|x| ClearFilters[Clear All Rules<br/>Show All Tracks]
    ClearFilters --> UpdateView
    
    WaitInput -->|s| SavePlaylist[Input Playlist Name<br/>Save activeRules]
    SavePlaylist --> StorePL[Store Playlist<br/>rules only, not songs]
    StorePL --> WaitInput
    
    WaitInput -->|q| Exit([Exit App])
    
    subgraph Layers
        direction LR
        L1[Display Layer<br/>BrowserView] 
        L2[Filter Logic<br/>FilterEngine + Rule + Playlist]
        L3[Metadata Layer<br/>Library + Track]
    end
    
    L1 -.->|uses| L2
    L2 -.->|reads| L3
    
    note1["MVP: 5 fields = title, artist,<br/>genre, mood, energy<br/>Custom tag: year<br/>Extensible: add 'rating' to Track only"]
    note1 -.-> L3
```


Sequence Diagram - "User Presses f" Flow
```mermaid
sequenceDiagram
    actor User
    participant UI as BrowserView<br/>Display Layer
    participant FE as FilterEngine<br/>Logic Layer  
    participant Lib as Library<br/>Metadata Layer
    participant PL as Playlist Manager

    User->>UI: Press 'f'
    UI->>User: Prompt: Enter keyword or rule

    alt User types "kpop"
        UI->>FE: applyRules(genre == "kpop")
        FE->>Lib: getAllTracks()
        Lib-->>FE: List<Track>
        FE-->>PL: Create Playlist "Kpop"
        PL-->>UI: Show Kpop Playlist
    else User types "rap"
        UI->>FE: applyRules(genre == "rap")
        FE->>Lib: getAllTracks()
        Lib-->>FE: List<Track>
        FE-->>PL: Create Playlist "Rap"
        PL-->>UI: Show Rap Playlist
    else User types "energy <= 5"
        UI->>FE: applyRules(energy <= 5)
        FE->>Lib: getAllTracks()
        Lib-->>FE: List<Track>
        FE-->>PL: Create Playlist "Relapse/Sad Songs"
        PL-->>UI: Show Relapse/Sad Songs Playlist
    else User types "energy >= 6"
        UI->>FE: applyRules(energy >= 6)
        FE->>Lib: getAllTracks()
        Lib-->>FE: List<Track>
        FE-->>PL: Create Playlist "Rap Songs"
        PL-->>UI: Show Rap Songs Playlist
    end
```

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "FilterEngine.h"
#include "Library.h"

TEST_CASE("FilterEngine applies rules") {
    Library lib;
    lib.loadFromJson("LIBRARY.json");
    FilterEngine fe;

    fe.addRule(Rule("genre", "==", "Rap"));
    auto filtered = fe.apply(lib.getAllTracks());

    for (auto& track : filtered) {
        CHECK(track.getGenre() == "Rap");
    }
}

TEST_CASE("FilterEngine loads presets") {
    FilterEngine fe;
    bool ok = fe.loadPresetLibrary("F_Rule.json");
    CHECK(ok);

    bool applied = fe.applyPresetMatchingInput("Rap Songs");
    CHECK(applied);
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Library.h"

TEST_CASE("Playlist tracks appear first") {
    Library lib;
    lib.loadFromJson("LIBRARY.json");

    auto firstTrack = lib.getTrack(0);
    // Playlist.json ensures Sirena is first
    CHECK(firstTrack.getTitle() == "Sirena");
}

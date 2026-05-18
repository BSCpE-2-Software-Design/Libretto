#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Library.h"

TEST_CASE("Library loads JSON") {
    Library lib;
    lib.loadFromJson("LIBRARY.json"); // sample file must exist

    CHECK(lib.size() > 0);

    auto track = lib.getTrack(0);
    CHECK(track.getTitle().size() > 0);
}

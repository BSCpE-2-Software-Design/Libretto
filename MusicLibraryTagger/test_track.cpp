#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Track.h"

TEST_CASE("Track getters and JSON") {
    Track t("Sirena", "Gloc-9", "Rap", "Love", 8, "sirena.mp3");

    CHECK(t.getTitle() == "Sirena");
    CHECK(t.getArtist() == "Gloc-9");
    CHECK(t.getGenre() == "Rap");
    CHECK(t.getMood() == "Love");
    CHECK(t.getEnergy() == 8);
    CHECK(t.getFilePath() == "sirena.mp3");

    auto j = t.toJson();
    Track t2 = Track::fromJson(j);
    CHECK(t2.getTitle() == "Sirena");
    CHECK(t2.getArtist() == "Gloc-9");
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Rule.h"
#include "Track.h"

TEST_CASE("Rule matches track") {
    Track t("Sirena", "Gloc-9", "Rap", "Love", 8, "sirena.mp3");

    Rule r1("genre", "==", "Rap");
    CHECK(r1.matches(t));

    Rule r2("artist", "==", "Gloc-9");
    CHECK(r2.matches(t));

    Rule r3("energy", ">=", "5");
    CHECK(r3.matches(t));

    Rule r4("mood", "==", "Love");
    CHECK(r4.matches(t));
}

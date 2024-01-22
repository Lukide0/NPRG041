#include "utils.h"
#include <doctest.h>
#include <koterm/screen/Tile.h>

using namespace koterm::screen;

TEST_CASE("Tiles: Simple combine") {

    std::uint8_t code;
    KOTERM_TEST_GENERATOR(code, 1, 4) {

        const auto top    = TileEncoding({ code, 0, 0, 0 });
        const auto right  = TileEncoding({ 0, code, 0, 0 });
        const auto bottom = TileEncoding({ 0, 0, code, 0 });
        const auto left   = TileEncoding({ 0, 0, 0, code });

        // 1/4
        CHECK_EQ(left | left, left);
        CHECK_EQ(right | right, right);
        CHECK_EQ(top | top, top);
        CHECK_EQ(bottom | bottom, bottom);

        // 2/4
        CHECK_EQ(left | top, TileEncoding { code, 0, 0, code });
        CHECK_EQ(left | right, TileEncoding { 0, code, 0, code });
        CHECK_EQ(left | bottom, TileEncoding { 0, 0, code, code });

        CHECK_EQ(right | top, TileEncoding { code, code, 0, 0 });
        CHECK_EQ(right | bottom, TileEncoding { 0, code, code, 0 });

        CHECK_EQ(top | bottom, TileEncoding { code, 0, code, 0 });

        // 3/4
        CHECK_EQ(right | bottom | top, TileEncoding { code, code, code, 0 });
        CHECK_EQ(right | left | top, TileEncoding { code, code, 0, code });
        CHECK_EQ(bottom | left | top, TileEncoding { code, 0, code, code });
        CHECK_EQ(bottom | left | right, TileEncoding { 0, code, code, code });

        // 4/4
        CHECK_EQ(top | right | bottom | left, TileEncoding { code, code, code, code });
    }
}

TEST_CASE("Tile: Complex combine") {

    std::uint8_t encoding;
    KOTERM_TEST_GENERATOR(encoding, 0, 256) {
        TileEncoding tile { encoding };

        // HACK: Combine not valid tile with itself
        tile = tile | tile;

        INFO(" ", tile.top(), " ", tile.right(), " ", tile.bottom(), " ", tile.left());
        CHECK_NE(TILES_MAP.find(tile), TILES_MAP.npos);
    }
}

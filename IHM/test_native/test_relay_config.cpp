#include "mini_test.h"
#include "RelayConfig.h"

TEST(RelayConfig, DefaultsToOff) {
    RelayConfig cfg;
    CHECK_FALSE(cfg.state);
}

TEST(RelayConfig, ToggleFlipsState) {
    RelayConfig cfg;
    cfg.toggle();
    CHECK_TRUE(cfg.state);
    cfg.toggle();
    CHECK_FALSE(cfg.state);
}

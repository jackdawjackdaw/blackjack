cmake_minimum_required(VERSION 2.8)

add_library(blackjack cards deck useful hand)

add_executable(simple-jack game player-actions)
target_link_libraries(simple-jack blackjack)

install_targets(/bin simple-jack)

## packagemaker doesn't work on my laptop
SET(CPACK_BINARY_PACKAGEMAKER "OFF")
include(cpack)
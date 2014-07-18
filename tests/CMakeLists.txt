enable_testing()
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIR})

## add the lib we want to test in here
set(libs ${libs} ${CHECK_LIBRARIES} blackjack)
include_directories(. ../src)
## now build the test executable
add_executable(test-cards test-cards)
target_link_libraries(test-cards ${libs})

add_executable(test-deck test-deck)
target_link_libraries(test-deck ${libs})


add_test(test_cards ${CMAKE_CURRENT_BINARY_DIR}/test-cards)

add_test(test_deck ${CMAKE_CURRENT_BINARY_DIR}/test-deck)

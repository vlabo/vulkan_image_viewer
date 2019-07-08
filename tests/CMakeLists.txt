add_executable(
    unit_tests
    main.cpp
    )

target_link_libraries(
    unit_tests
    gtest_main
    )

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/tests/unit_tests
  )
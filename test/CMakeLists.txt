add_executable(test_main
    test_main.cpp
        test_polynom.cpp
)

target_include_directories(test_main PRIVATE
    ${CMAKE_SOURCE_DIR}/polynomial/include
    ${CMAKE_SOURCE_DIR}/polynomial/gtest
)

target_link_libraries(test_main PRIVATE
    gtest
    polynomial_includes
)


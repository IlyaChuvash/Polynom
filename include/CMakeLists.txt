add_library(polynomial_includes INTERFACE
        polynomial.h
    list.h
)

target_include_directories(polynomial_includes INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)


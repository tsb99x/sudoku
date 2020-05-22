add_library(layout
        layout.c)
target_include_directories(layout PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(layout
        SDL2
        SDL2main)

add_executable(test_layout WIN32
        test_layout.c)
target_link_libraries(test_layout
        layout)
add_test(NAME layout
        COMMAND test_layout)

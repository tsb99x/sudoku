# Internal Libraries

add_library(context
        context.c
        context.h)
target_link_libraries(context
        SDL2
        SDL2_ttf)

add_library(layout
        layout.c
        layout.h)
target_link_libraries(layout
        SDL2)

add_library(grid
        grid.c
        grid.h)

add_library(button
        button.c
        button.h)
target_link_libraries(button
        context
        grid
        SDL2)

add_library(palette
        palette.c
        palette.h)
target_link_libraries(palette
        SDL2)

# Application

set(EXT_LIBS
        SDL2
        SDL2main
        SDL2_ttf)
set(INT_LIBS
        button
        context
        grid
        layout
        palette)
add_executable(${PROJECT_NAME} WIN32
        main.c)
target_link_libraries(${PROJECT_NAME}
        ${EXT_LIBS}
        ${INT_LIBS})

foreach(LIB SDL2 SDL2_ttf FreeType ZLib)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo
                Copy $<TARGET_FILE:${LIB}> into $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${LIB}> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach()

# Tests

add_executable(layout_test
        layout_test.c)
target_link_libraries(layout_test
        layout
        SDL2main)
add_test(NAME layout
        COMMAND layout_test)

add_executable(grid_test
        grid_test.c)
target_link_libraries(grid_test
        grid)
add_test(NAME grid
        COMMAND grid_test)

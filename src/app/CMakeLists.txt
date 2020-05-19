add_executable(${PROJECT_NAME} WIN32
        main.c)
target_link_libraries(${PROJECT_NAME}
        SDL2
        SDL2_ttf)

foreach(LIB SDL2 SDL2_ttf FreeType ZLib)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo
                Copy $<TARGET_FILE:${LIB}> into $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${LIB}> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach()

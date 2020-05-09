add_executable(${PROJECT_NAME} WIN32 main.c)
target_link_libraries(${PROJECT_NAME} SDL2)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

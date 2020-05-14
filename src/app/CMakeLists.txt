find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${PROJECT_NAME} WIN32
        main.c)
target_link_libraries(${PROJECT_NAME}
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf)
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if(${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(ARCH_DIR x64)
else()
        set(ARCH_DIR x86)
endif()

set(SDL_VER 2.0.12)
set(TTF_VER 2.0.15)

foreach(ZIP SDL2-devel-${SDL_VER}-VC SDL2_ttf-devel-${TTF_VER}-VC)
        message("Unpacking ${ZIP}")
        execute_process(COMMAND ${CMAKE_COMMAND}
                -E tar xzf ${CMAKE_CURRENT_LIST_DIR}/${ZIP}.zip
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

set(SDL_DIR ${CMAKE_CURRENT_BINARY_DIR}/SDL2-${SDL_VER})
set(TTF_DIR ${CMAKE_CURRENT_BINARY_DIR}/SDL2_ttf-${TTF_VER})

add_library(SDL2 SHARED IMPORTED GLOBAL)
set_target_properties(SDL2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${SDL_DIR}/include
        IMPORTED_LOCATION ${SDL_DIR}/lib/${ARCH_DIR}/SDL2.dll
        IMPORTED_IMPLIB ${SDL_DIR}/lib/${ARCH_DIR}/SDL2.lib)

add_library(SDL2main STATIC IMPORTED GLOBAL)
set_target_properties(SDL2main PROPERTIES
        IMPORTED_LOCATION ${SDL_DIR}/lib/${ARCH_DIR}/SDL2main.lib)

add_library(SDL2_ttf SHARED IMPORTED GLOBAL)
set_target_properties(SDL2_ttf PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${TTF_DIR}/include
        IMPORTED_LOCATION ${TTF_DIR}/lib/${ARCH_DIR}/SDL2_ttf.dll
        IMPORTED_IMPLIB ${TTF_DIR}/lib/${ARCH_DIR}/SDL2_ttf.lib)

add_library(FreeType SHARED IMPORTED GLOBAL)
set_target_properties(FreeType PROPERTIES
        IMPORTED_LOCATION ${TTF_DIR}/lib/${ARCH_DIR}/libfreetype-6.dll)

add_library(ZLib SHARED IMPORTED GLOBAL)
set_target_properties(ZLib PROPERTIES
        IMPORTED_LOCATION ${TTF_DIR}/lib/${ARCH_DIR}/zlib1.dll)

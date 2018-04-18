include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/deps
        ${CMAKE_SOURCE_DIR}/src/deps/freetype
        ${CMAKE_SOURCE_DIR}/src/deps/ogg
        ${CMAKE_SOURCE_DIR}/src/deps/zlib
        ${CMAKE_SOURCE_DIR}/src/deps/vorbis
        ${CMAKE_SOURCE_DIR}/src/deps/FLAC
        ${CMAKE_SOURCE_DIR}/src/deps/sdl/include
        ${CMAKE_SOURCE_DIR}/src/deps/sdl_mixer
        ${CMAKE_SOURCE_DIR}/src/deps/stb_image
        ${CMAKE_SOURCE_DIR}/src/deps/kazmath
        )

file(GLOB CORE_SOURCE core/*.c core/*.h)
file(GLOB REMOVE_CORE_SOURCE
        "core/binocle.c"
        )
list(REMOVE_ITEM CORE_SOURCE ${REMOVE_CORE_SOURCE})
file(GLOB UNITY_SOURCE core/binocle.c core/binocle.h)

if (APPLE)
    set(PLATFORM_SOURCE core/PlayerPrefs.mm)
endif ()

set(SOURCE_FILES
        ${CORE_SOURCE}
        ${PLATFORM_SOURCE}
        )

set(UNITY_FILES
        ${UNITY_SOURCE}
        ${PLATFORM_SOURCE}
        )

GroupSources("core")

# Uncomment the following line to avoid unity builds
add_library(binocle ${SOURCE_FILES})
# Uncomment the following line to use unity builds
#add_library(binocle ${UNITY_FILES})

if (IOS)
    set_target_properties(
            binocle
            PROPERTIES
            XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0
    )
endif(IOS)
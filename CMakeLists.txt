# template: https://github.com/SFML/cmake-sfml-project/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.21)
project(yaya LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(yaya
        src/yaya.cpp
        src/camera.cpp
        src/camera.cpp
        src/controls.cpp
        src/debugger.cpp
        src/duck.cpp
        src/entity.cpp
        src/events.cpp
        src/game.cpp
        src/graphics.cpp
        src/neighborsfinder.cpp
        src/player.cpp
        src/egg.cpp
        src/timer.cpp
        src/utilities.cpp
        src/map.cpp
        src/ui.cpp)

target_link_libraries(yaya PRIVATE sfml-graphics)
target_compile_features(yaya PRIVATE cxx_std_17)

add_custom_command(TARGET yaya POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:yaya> $<TARGET_FILE_DIR:yaya> COMMAND_EXPAND_LISTS)

install(TARGETS yaya)
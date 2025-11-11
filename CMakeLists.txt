cmake_minimum_required(
    VERSION 3.15
)

project(Hydrion
    VERSION 0.0.1
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(hydrion
    src/main.cpp
)

target_include_directories(hydrion PRIVATE
    include/
    src/
)

# if(UNIX AND NOT APPLE)
#     find_package(X11 REQUIRED)
#     target_link_libraries(game X11)
# endif()
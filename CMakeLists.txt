cmake_minimum_required(VERSION 3.16)
project(FileExplorer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

add_library(backend
    backend/file_operations.cpp
    backend/settings.cpp
)
target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backend)
target_link_libraries(backend PRIVATE nlohmann_json::nlohmann_json)

add_executable(FileExplorer
    gui/main.cpp
    gui/file_explorer.cpp
)

target_link_libraries(FileExplorer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    nlohmann_json::nlohmann_json
    backend
)


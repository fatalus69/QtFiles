cmake_minimum_required(VERSION 3.16)
project(QtFiles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml)

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

# Prevent GoogleTest from overriding compiler settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(backend
  backend/file_operations.cpp
  backend/settings.cpp
)
target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backend)
# Add QtCore so we can return QStrings
target_link_libraries(backend PUBLIC Qt6::Core)
target_link_libraries(backend PRIVATE nlohmann_json::nlohmann_json)

add_executable(QtFiles
  gui/main.cpp
  gui/file_explorer.cpp
  gui/builders/modal_builder.cpp
)

target_link_libraries(QtFiles PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Qml
  nlohmann_json::nlohmann_json
  backend
)

enable_testing()

add_executable(run_tests 
  tests/test_utils.cpp
  tests/test_file_operations.cpp
)
target_link_libraries(run_tests PRIVATE backend GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)

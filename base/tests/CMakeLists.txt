cmake_minimum_required(VERSION 3.15)
project(GeometriaBase-tests)

set(
    SOURCES
    "2d.cpp"
    "Event.cpp"
    "Observable.cpp"
    "path.cpp"
    "Plane.cpp"
    "tests.cpp"
    "Weak.cpp"
)

set(all_code_files
    ${SOURCES}
)

add_executable(GeometriaBase-tests ${all_code_files})

include_directories("${PROJECT_BINARY_DIR}")

foreach(SOURCE IN LISTS all_code_files)
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    string(REPLACE "/" "\\" SOURCE_GROUP "${SOURCE_PATH}")
    source_group("${SOURCE_GROUP}" FILES "${SOURCE}")
endforeach()

target_compile_features(GeometriaBase-tests PRIVATE cxx_std_17)

target_link_libraries(GeometriaBase-tests PUBLIC geometria::base)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(GeometriaBase-tests PRIVATE GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(GeometriaBase-tests)

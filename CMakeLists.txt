cmake_minimum_required(VERSION 3.20)
project(AlgorithmValidationEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# ---- cpp-httplib (single-header HTTP server) ----
FetchContent_Declare(httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG        v0.18.3
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(httplib)

# ---- nlohmann/json (header-only JSON) ----
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(nlohmann_json)

# ---- SQLiteCpp (bundles sqlite3) ----
set(SQLITECPP_RUN_CPPLINT  OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_TESTS  OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG        3.3.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SQLiteCpp)

# ---- Google Test ----
set(BUILD_GMOCK   OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

# ---- Algorithm library (shared between server and tests) ----
add_library(ave_lib STATIC
    src/algorithms/KalmanFilter.cpp
    src/algorithms/LeastSquares.cpp
    src/db/Repository.cpp
    src/api/Router.cpp
)
target_include_directories(ave_lib PUBLIC include)
target_link_libraries(ave_lib
    PUBLIC
        httplib::httplib
        nlohmann_json::nlohmann_json
        SQLiteCpp
        Threads::Threads
)

# ---- Server executable ----
add_executable(ave_server src/main.cpp)
target_link_libraries(ave_server PRIVATE ave_lib)

# ---- Test executable ----
add_executable(ave_tests
    test/kalman_test.cpp
    test/leastsq_test.cpp
)
target_link_libraries(ave_tests PRIVATE ave_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(ave_tests)
enable_testing()

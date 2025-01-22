cmake_minimum_required(VERSION 3.30...3.31)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
project(cpp_url_short CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(
## SQLite database file
    -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/urlMapDb.sqlite"
)


find_package(oatpp CONFIG REQUIRED)
find_package(oatpp-sqlite CONFIG REQUIRED)

include_directories( ${oatpp_INCLUDE_DIRS})
add_executable(cpp_url_short main.cpp)
target_link_libraries(
    cpp_url_short PRIVATE 
    oatpp::oatpp
    oatpp::oatpp-sqlite
    oatpp::oatpp-test
)

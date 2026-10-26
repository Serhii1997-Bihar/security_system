cmake_minimum_required(VERSION 3.28)
set(VCPKG_MANIFEST_INSTALL_OPTIONS "--no-binarycaching")
project(server_core)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt CONFIG REQUIRED)
find_package(Botan CONFIG REQUIRED)
find_package(Drogon CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(server_core
        main.cpp)

target_link_libraries(
        server_core
        PRIVATE
        fmt::fmt
        nlohmann_json::nlohmann_json
        Botan::Botan
        Drogon::Drogon
)

target_include_directories(
        server_core
        PRIVATE
        include
)
cmake_minimum_required(VERSION 3.28)

project(server-core CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt CONFIG REQUIRED)
find_package(Botan CONFIG REQUIRED)
find_package(Drogon CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)

add_executable(server-core
        main.cpp
        include/security/auth_filter.hpp
        src/helpers/load_env.cpp
        include/helpers/load_env.hpp
        include/models/security.hpp
        src/security/websocket.cpp
        include/security/websocket.hpp
        src/security/auth_filter.cpp
)

target_include_directories(
        server-core
        PRIVATE
        include
)

target_link_libraries(
        server-core
        PRIVATE
        fmt::fmt
        nlohmann_json::nlohmann_json
        Botan::Botan
        Drogon::Drogon
        OpenSSL::Crypto
        jwt-cpp::jwt-cpp
)
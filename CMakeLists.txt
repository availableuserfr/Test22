cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

include($ENV{GEODE_SDK}/cmake/GeodeFile.cmake)

project(RestartLock VERSION 1.0.0 LANGUAGES CXX)

setup_geode_mod(RestartLock)

cmake_minimum_required(VERSION 3.30)
project(Supemon C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(include/cJSON)

add_executable(Supemon main.c 
    src/supemon.c 
    src/battle.c 
    src/shop.c 
    src/save.c 
    src/utils.c 
    src/item.c
    src/center.c
    src/display.c
    include/cJSON/cJSON.c)

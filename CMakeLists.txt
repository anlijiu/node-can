cmake_minimum_required(VERSION 3.9)

project(can)

add_subdirectory(3rdparty/endian)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN)
#set(CMAKE_CXX_FLAGS "-fsanitize=address")

#set(CMAKE_CXX_FLAGS "-fPIC --std=c++17 -fpermissive -Wall -Wextra -lpthread")
set(NAPI_CPP_EXCEPTIONS "yes")

file(GLOB SOURCE_FILES
        "src/*.h"
        "src/*.cc"
        "src/can/*.cc"
        "src/can/*.h"
        "src/weapon/*.h"
        "src/weapon/*.cc"
        "src/weapon/ammo/*.h"
        "src/weapon/ammo/*.cc"
        "src/tools/*.h"
        "src/tools/*.cc" 
        )

include_directories(
    ${CMAKE_SOURCE_DIR}/build
    ${CMAKE_SOURCE_DIR}/src
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api
        ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_JS_LIB} ${CMAKE_SOURCE_DIR}/lib/libcontrolcan.so )

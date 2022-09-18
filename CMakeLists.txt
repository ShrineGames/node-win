cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0042 NEW)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-DNAPI_VERSION=3)
project(node_win)

set(SOURCES 
        src/library.cpp 
        src/service.h 
        src/service.cpp)

add_library(node_win SHARED ${SOURCES} ${CMAKE_JS_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

set_target_properties(node_win PROPERTIES
        PREFIX ""
        OUTPUT_NAME "node_win"
        SUFFIX ".node"
        POSITION_INDEPENDENT_CODE ON)

execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(node_win PRIVATE ${NODE_ADDON_API_DIR})
cmake_minimum_required(VERSION 3.5)

project(ws_client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS WebSockets Widgets REQUIRED)

set(PROJECT_SOURCES
  src/main.cpp
  src/widget.cpp
  src/widget.h
)

#add_executable(...
qt_add_executable(ws_client
  ${PROJECT_SOURCES}
)

target_link_libraries(ws_client PRIVATE
  Qt6::WebSockets
  Qt6::Widgets
)

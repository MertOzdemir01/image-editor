cmake_minimum_required(VERSION 3.10)

project(ImagEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(MySQL REQUIRED)
find_package(PkgConfig REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${MYSQL_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIRS})

pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Source files
set(SOURCES
    main.cpp
    mainwindow.cpp
    imageprocessingservice.cpp
)

# Header files
set(HEADERS
    mainwindow.h
    imageprocessingservice.h
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UIS} ${RESOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    ${OpenCV_LIBS}
    ${MYSQL_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    Boost::system
    Boost::thread)

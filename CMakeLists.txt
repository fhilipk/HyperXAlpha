cmake_minimum_required(VERSION 3.10)

project(HyperX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(hidapi REQUIRED)

add_executable(HyperX
    src/hyperxApp.cpp
    src/hyperxFrame.cpp
    src/SwitchCtrl.cpp
    src/dialog.cpp
)

include(${wxWidgets_USE_FILE})

target_include_directories(HyperX PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(HyperX PRIVATE
    hidapi::hidapi
    ${wxWidgets_LIBRARIES}
)

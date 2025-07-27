cmake_minimum_required(VERSION 3.16)

# Optional: Set specific compilers (uncomment if needed)
# if(APPLE)
#     set(CMAKE_C_COMPILER /usr/local/bin/gcc-15)
#     set(CMAKE_CXX_COMPILER /usr/local/bin/g++-15)
# endif()

project(Qt5DataSelector VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags for macOS
if(APPLE)
    # Remove the old clang-specific flags since we're using GCC now
    # GCC doesn't need -stdlib=libc++
endif()

# Find Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Set Qt5 specific settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/TimeWidget.cpp
    src/DeviceManager.cpp
    src/DeviceWidget.cpp
    src/CustomButton.cpp
)

# Header files
set(HEADERS
    include/MainWindow.h
    include/TimeWidget.h
    include/DeviceManager.h
    include/DeviceInfo.h
    include/DeviceWidget.h
    include/CustomButton.h
)

# Resources
set(RESOURCES
    resources.qrc
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

# Link Qt5 libraries
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources 
     DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Optional: Enable testing (uncomment to enable tests)
# enable_testing()
# add_subdirectory(tests)
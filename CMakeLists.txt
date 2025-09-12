cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "asteroidDestroyer")
project(${PROJECT_NAME})

# SFML
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

# Source files
file(GLOB SRC_FILES src/*.c)

# Executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include headers
target_include_directories(${PROJECT_NAME} PUBLIC includes)

# Link SFML
target_link_libraries(${PROJECT_NAME} 
  csfml-system 
  csfml-window 
  csfml-graphics 
  csfml-audio 
  csfml-network
)

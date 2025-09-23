# Minimální verze CMake
cmake_minimum_required(VERSION 3.5)

# Konstanty
set(PROJECT_NAME "asteroidDestroyer")

# Nastavení názvu projektu
project(${PROJECT_NAME})

# Balíčky (nepovinné, ale podstatné)
find_package(SFML 2.6 COMPONENTS system window graphics audio network REQUIRED)

# Tvorba executable
add_executable(
  ${PROJECT_NAME}
  src/entities/asteroid.c
  src/entities/sprites.c
  src/graphics/window.c
  src/graphics/color.c
  src/core/time.c
  src/core/movement.c
  src/audio/sound.c
  src/main.c
)
target_link_directories(${PROJECT_NAME} PUBLIC src entities graphics core audio)
target_link_libraries(${PROJECT_NAME} csfml-system csfml-window csfml-graphics csfml-audio csfml-network m)
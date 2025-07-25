cmake_minimum_required(VERSION 3.14)
project(BaudoEngine C)

# Utilise le standard C99
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Inclure les headers
include_directories(include)

# Récupère tous les fichiers .c dans src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)

# Crée l'exécutable
add_executable(prog ${SOURCES})

# === Trouve et lie SDL2 ===
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(prog ${SDL2_LIBRARIES})

# === OpenGL ===
find_package(OpenGL REQUIRED)
target_link_libraries(prog OpenGL::GL)

# === GLEW ===
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(prog ${GLEW_LIBRARIES})

# === CGLM ===
find_path(CGLM_INCLUDE_DIR cglm/cglm.h)
if(NOT CGLM_INCLUDE_DIR)
    message(FATAL_ERROR "CGLM not found. Install libcglm-dev or provide the include path.")
endif()
include_directories(${CGLM_INCLUDE_DIR})

# === libm (math) ===
find_library(M_LIB m)
if(M_LIB)
    target_link_libraries(prog ${M_LIB})
else()
    message(WARNING "libm (math.h) not found. Some math functions might fail to link.")
endif()

# === Répertoire de sortie ===
set_target_properties(prog PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)


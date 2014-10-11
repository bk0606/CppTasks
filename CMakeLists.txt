cmake_minimum_required(VERSION 2.8.4)

### App name
project(CppTasks)
set (APP_NAME CppTasks)

## Sources directory
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)

### Source code
### recursive includes all files in SRC_DIR
file(GLOB_RECURSE SRC ${SRC_DIR} ${SRC_DIR}/*)
FOREACH(_FILE ${SRC})
  IF(IS_DIRECTORY ${_FILE})
    INCLUDE_DIRECTORIES(${_FILE})
  ENDIF()
ENDFOREACH()
add_executable ( ${APP_NAME} ${SRC} )

### Build types. May be: DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set( CMAKE_BUILD_TYPE DEBUG )

### Build directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/builds )

### Libs dirs
#link_directories ( /path/to/lib_1 )
#link_directories ( ${PROJECT_SOURCE_DIR}/libs/FreeImage/FreeImage )

### Libs names
#target_link_libraries( ${APP_NAME} name_1 )
#target_link_libraries( ${APP_NAME} FreeImage )

##################

### OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


### OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

### GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

# create the openGL program "test"
target_link_libraries(${APP_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )